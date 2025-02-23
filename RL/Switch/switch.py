#!/usr/bin/python3
import sys
import struct
import wrapper
import threading
import time
from wrapper import recv_from_any_link, send_to_link, get_switch_mac, get_interface_name

my_root_bridge_id = 0
my_sender_bridge_id = 0
my_sender_path_cost = 0

def parse_ethernet_header(data):
    # Unpack the header fields from the byte array
    #dest_mac, src_mac, ethertype = struct.unpack('!6s6sH', data[:14])
    dest_mac = data[0:6]
    src_mac = data[6:12]
    
    # Extract ethertype. Under 802.1Q, this may be the bytes from the VLAN TAG
    ether_type = (data[12] << 8) + data[13]

    vlan_id = -1
    # Check for VLAN tag (0x8100 in network byte order is b'\x81\x00')
    if ether_type == 0x8200:
        vlan_tci = int.from_bytes(data[14:16], byteorder='big')
        vlan_id = vlan_tci & 0x0FFF  # extract the 12-bit VLAN ID
        ether_type = (data[16] << 8) + data[17]

    return dest_mac, src_mac, ether_type, vlan_id

def create_vlan_tag(vlan_id):
    # 0x8100 for the Ethertype for 802.1Q
    # vlan_id & 0x0FFF ensures that only the last 12 bits are used
    return struct.pack('!H', 0x8200) + struct.pack('!H', vlan_id & 0x0FFF)

def send_bdpu_every_sec(vlan_table, interfaces):
    while True:
        # TODO Send BDPU every second if necessary
        if my_sender_bridge_id == my_root_bridge_id:
            bpdu = create_bpdu(my_root_bridge_id, my_sender_bridge_id, my_sender_path_cost)
            for interface in interfaces:
                if vlan_table[get_interface_name(interface)] == "T":
                    send_to_link(interface, len(bpdu), bpdu)
        time.sleep(1)

def create_bpdu(root_bridge_id, sender_bridge_id, sender_path_cost):
    dest_mac = b'\x01\x80\xc2\x00\x00\x00'
    root_bridge_id = struct.pack("!q", root_bridge_id)
    sender_bridge_id = struct.pack("!q", sender_bridge_id)
    sender_path_cost = struct.pack("!I", sender_path_cost)
    return dest_mac + root_bridge_id + sender_bridge_id + sender_path_cost 

def is_unicast(mac):
    first_octet = mac[0]
    return (first_octet & 1) == 0

def forward(data, length, in_interface, out_interface, vlan_table):
    dest_mac, src_mac, ethertype, vlan_id = parse_ethernet_header(data)
    if vlan_id == -1:
        vlan_id = int(vlan_table[get_interface_name(in_interface)])
    if vlan_table[get_interface_name(out_interface)] == "T":
        if vlan_table[get_interface_name(in_interface)] == "T":
            send_to_link(out_interface, length, data)
        else:
            forward_to_trunk(data, length, in_interface, out_interface, vlan_table, vlan_id)
    elif vlan_id == int(vlan_table[get_interface_name(out_interface)]):
        if vlan_table[get_interface_name(in_interface)] == "T":
            forward_to_access(data, length, in_interface, out_interface)
        else:
            send_to_link(out_interface, length, data)


def forward_to_trunk(data, length, in_int, out_int, vlan_table, vlan_id):
    tagged_frame = data[0:12] + create_vlan_tag(int(vlan_id)) + data[12:]
    send_to_link(out_int, length + 4, tagged_frame)

def forward_to_access(data, length, in_int, out_int):
    untagged_frame = data[0:12] + data[16:]
    send_to_link(out_int, length - 4, untagged_frame)


def main():
    # init returns the max interface number. Our interfaces
    # are 0, 1, 2, ..., init_ret value + 1
    switch_id = sys.argv[1]

    num_interfaces = wrapper.init(sys.argv[2:])
    interfaces = range(0, num_interfaces)

    print("# Starting switch with id {}".format(switch_id), flush=True)
    print("[INFO] Switch MAC", ':'.join(f'{b:02x}' for b in get_switch_mac()))


    # Printing interface names
    for i in interfaces:
        print(get_interface_name(i))
        print(i)
        
    #cam table
    cam_table = {}

    #vlan table
    vlan_table = {}
    file = open(f"./configs/switch{switch_id}.cfg", "r")
    priority = file.readline().strip()
    priority = int(priority)

    for i in interfaces:
        name, mode = file.readline().strip().split()[0:2]
        vlan_table.update({name: mode})

    file.close()

    # # PRINT VLAN TABLE
    # for i in vlan_table:
    #     print(f"Interface {i} is in VLAN {vlan_table[i]}")


    global my_root_bridge_id, my_sender_bridge_id, my_sender_path_cost
    stp_table = {}
    my_root_bridge_id = priority
    my_sender_bridge_id = priority

    for interface in interfaces:
        stp_table[interface] = "L"


    # Create and start a new thread that deals with sending BDPU
    t = threading.Thread(target=send_bdpu_every_sec, args=(vlan_table, interfaces,))
    t.start()

    while True:
        # Note that data is of type bytes([...]).
        # b1 = bytes([72, 101, 108, 108, 111])  # "Hello"
        # b2 = bytes([32, 87, 111, 114, 108, 100])  # " World"
        # b3 = b1[0:2] + b[3:4].
        interface, data, length = recv_from_any_link()

        dest_mac, src_mac, ethertype, vlan_id = parse_ethernet_header(data)


        #STP
        if data[0:6] == b'\x01\x80\xc2\x00\x00\x00':
            print("Received BDPU")
            bdpu_root_bridge_id = data[6:14]
            bdpu_root_bridge_id = int.from_bytes(bdpu_root_bridge_id, byteorder='big')
            bdpu_sender_bridge_id = data[14:22]
            bdpu_sender_bridge_id = int.from_bytes(bdpu_sender_bridge_id, byteorder='big')
            bdpu_sender_path_cost = data[22:26]
            bdpu_sender_path_cost = int.from_bytes(bdpu_sender_path_cost, byteorder='big')

            if bdpu_root_bridge_id < my_root_bridge_id:
                if my_root_bridge_id == my_sender_bridge_id:
                    for port in stp_table:
                        if vlan_table[get_interface_name(port)] == "T":
                            stp_table[port] = "B"
                 

                my_root_bridge_id = bdpu_root_bridge_id
                my_sender_path_cost = bdpu_sender_path_cost + 10

                if stp_table[interface] == "B":
                    stp_table[interface] = "L"
                
                bpdu = create_bpdu(my_root_bridge_id, my_sender_bridge_id, my_sender_path_cost)
                for i in interfaces:
                    if i != interface and vlan_table[get_interface_name(i)] == "T":
                        send_to_link(i, len(bpdu), bpdu)
            
            elif bdpu_root_bridge_id == my_root_bridge_id:
                if stp_table[interface] == "L":
                    if bdpu_sender_path_cost + 10 < my_sender_path_cost:
                        my_sender_path_cost = bdpu_sender_path_cost + 10
                
                elif bdpu_sender_path_cost > my_sender_path_cost:
                    stp_table[interface] = "L"
            
            elif bdpu_sender_bridge_id == my_sender_bridge_id:
                stp_table[interface] = "B"
            
            if my_sender_bridge_id == my_root_bridge_id:
                for port in stp_table:
                    if vlan_table[get_interface_name(port)] == "T":
                        stp_table[port] = "L"

            continue

        #CAM Table and VLAN
        in_interface = interface
        cam_table[src_mac] = in_interface
        if is_unicast(dest_mac):
            if dest_mac in cam_table:
                #unicast
                out_interface = cam_table[dest_mac]
                forward(data, length, in_interface, out_interface, vlan_table)
            else:
                #broadcast
                for i in interfaces:
                    if i != in_interface and stp_table[i] != "B":
                        forward(data, length, in_interface, i, vlan_table)
        else:
            #broadcast
            for i in interfaces:
                if i != in_interface and stp_table[i] != "B":
                    forward(data, length, in_interface, i, vlan_table)

        # # Print the MAC src and MAC dst in human readable format
        # dest_mac = ':'.join(f'{b:02x}' for b in dest_mac)
        # src_mac = ':'.join(f'{b:02x}' for b in src_mac)

        # # Note. Adding a VLAN tag can be as easy as
        # # tagged_frame = data[0:12] + create_vlan_tag(10) + data[12:]

        # print(f'Destination MAC: {dest_mac}')
        # print(f'Source MAC: {src_mac}')
        # print(f'EtherType: {ethertype}')

        # print("Received frame of size {} on interface {}".format(length, interface), flush=True)

        # TODO: Implement forwarding with learning
        # TODO: Implement VLAN support
        # TODO: Implement STP support

        # data is of type bytes.
        # send_to_link(i, length, data)

if __name__ == "__main__":
    main()
