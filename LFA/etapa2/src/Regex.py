from typing import Any, List
from .NFA import NFA

EPSILON = ''
special_chars = ['*', '|', '(', ')', '+', '?', '[', ']']
stack_ch = []
stack_opr = []
is_char = False
j = 0
class Regex:
    def thompson(self) -> NFA[int]:
        raise NotImplementedError('the thompson method of the Regex class should never be called')

# you should extend this class with the type constructors of regular expressions and overwrite the 'thompson' method
# with the specific nfa patterns. for example, parse_regex('ab').thompson() should return something like:

# >(0) --a--> (1) -epsilon-> (2) --b--> ((3))

# extra hint: you can implement each subtype of regex as a @dataclass extending Regex
          
class Character(Regex):
    def __init__(self, char): 
        self.char = char
    
    def thompson(self) -> NFA[int]:
        S = {self.char}
        K = {0, 1}
        q0 = 0
        d = {(0, self.char): {1}}
        F = {1}
        return NFA(S, K, q0, d, F)
    
class Concat(Regex):
    def __init__(self, left: Regex, right: Regex):
        self.left = left
        self.right = right

    def thompson(self) -> NFA[int]:
        nfa_left = self.left.thompson()
        nfa_right = self.right.thompson().remap_states(lambda x: x + len(nfa_left.K))
        S = nfa_left.S.union(nfa_right.S)
        K = nfa_left.K.union(nfa_right.K)
        q0 = nfa_left.q0
        d = nfa_left.d
        d.update(nfa_right.d)
        for state in nfa_left.F:
           d[(state, EPSILON)] = {nfa_right.q0}
        
        F = nfa_right.F
        return NFA(S, K, q0, d, F)

    
class Union(Regex):
    def __init__(self, left: Regex, right: Regex):
        self.left = left
        self.right = right

    def thompson(self) -> NFA[int]:
        nfa_left = self.left.thompson()
        nfa_right = self.right.thompson()
        S = nfa_left.S.union(nfa_right.S)
        K = nfa_left.K.union(nfa_right.K)

        q0 = max(K) + 1
        F = max(K) + 2
        K.add(q0)
        K.add(F)

        d = dict(nfa_left.d)
        d.update(nfa_right.d)
        d[(q0, EPSILON)] = {nfa_left.q0, nfa_right.q0}
        for state in nfa_left.F:
            d[(state, EPSILON)] = {F}
        for state in nfa_right.F:
            d[(state, EPSILON)] = {F}

        return NFA(S, K, q0, d, {F})

class Star(Regex):
    def __init__(self, regex: Regex):
        self.regex = regex

    def thompson(self) -> NFA[int]:
        nfa = self.regex.thompson()
        S = nfa.S
        K = nfa.K

        q0 = max(K) + 1
        F = max(K) + 2
        K.add(q0)
        K.add(F)

        d = nfa.d.copy()
        d[(q0, EPSILON)] = {nfa.q0, F}
        for state in nfa.F:
            d[(state, EPSILON)] = {nfa.q0, F}
        return NFA(S, K, q0, d, {F})
    
class Plus(Regex):
    def __init__(self, regex: Regex):
        self.regex = regex

    def thompson(self) -> NFA[int]:
        return self.regex.thompson()
    
class Question(Regex):
    def __init__(self, regex: Regex):
        self.regex = regex

    def thompson(self) -> NFA[int]:
        nfa = self.regex.thompson()
        S = nfa.S
        K = nfa.K
        q0 = nfa.q0
        d = nfa.d
        F = nfa.F | {q0}        
        return NFA(S, K, q0, d, F)

class Small(Regex):
    def __init__(self):
        self.S = set()
        self.q0 = 0
        self.d = {}
        self.F = {1}
        self.K = {0, 1}
        character = [chr(i) for i in range(97, 123)]
        for c in character:
            self.S.add(c)
        for c in character:
            self.d[(0, c)] = {1}

    def thompson(self) -> NFA[int]:
        return NFA(
            S = self.S,
            K = self.K,
            q0 = self.q0,
            d = self.d,
            F = self.F
        )
    
class Big(Regex):
    def __init__(self):
        self.S = set()
        self.q0 = 0
        self.d = {}
        self.F = {1}
        self.K = {0, 1}
        character = [chr(i) for i in range(65, 91)]
        for c in character:
            self.S.add(c)
        for c in character:
            self.d[(0, c)] = {1}
    
    def thompson(self) -> NFA[int]:
        return NFA(
            S = self.S,
            K = self.K,
            q0 = self.q0,
            d = self.d,
            F = self.F
        )

class Digit(Regex):
    def __init__(self):
        character = [Character(chr(i)) for i in range(48, 58)]
        self.regex = character[0]
        for i in range(1, 10):
            self.regex = Union(self.regex, character[i])

    def thompson(self) -> NFA[int]:
        return self.regex.thompson()


def parse_regex(regex: str) -> Regex:
    # create a Regex object by parsing the string

    # you can define additional classes and functions to help with the parsing process

    # the checker will call this function, then the thompson method of the generated object. the resulting NFA's
    # behaviour will be checked using your implementation form stage 1
    global stack_ch, stack_opr, is_char, j
    is_char = False
    i = 0
    while i < len(regex):
        if regex[i] == ' ':
            i += 1
            continue
        if regex[i] in special_chars:
            i = parse_special_char(regex[i], regex, i)
        else:
            if regex[i] == '\\' and i + 1 < len(regex):
                i += 1
                parse_char(regex[i], regex)
            else:
                parse_char(regex[i], regex)
            i += 1
            
    while stack_opr:
        apply_opr()
    
    return stack_ch.pop()

def parse_char(char: str, regex: str):
    global stack_ch, stack_opr, is_char
    if is_char:
        stack_opr.append('.')
    is_char = True
    stack_ch.append(Character(char))

def parse_special_char(char: str, regex: str, i: int):
    global stack_ch, stack_opr, is_char
    if char == '|':
        while stack_opr and stack_opr[-1] == '.':
            apply_opr()
        stack_opr.append('|')
        is_char = False
    elif char == '*':
        stack_ch.append(Star(stack_ch.pop()))
        is_char = True
    elif char == '+':
        stack_ch.append(Plus(stack_ch.pop()))
        is_char = True
    elif char == '?':
        stack_ch.append(Question(stack_ch.pop()))
        is_char = True
    elif char == '(':
        if is_char:
            stack_opr.append('.')
        stack_opr.append('(')
        is_char = False
    elif char == ')':
        while stack_opr[-1] != '(':
            apply_opr()
        stack_opr.pop()
        is_char = True
    elif char == '[':
        close = regex.find(']', i)

        range = regex[i + 1:close]
        if range == 'a-z':
            stack_ch.append(Small())
        elif range == 'A-Z':
            stack_ch.append(Big())
        elif range == '0-9':
            stack_ch.append(Digit())

        i = close
        if is_char:
            stack_opr.append('.')
        is_char = True
    i += 1
    return i

def apply_opr():
    global stack_ch, stack_opr
    right = stack_ch.pop()
    left = stack_ch.pop()
    opr = stack_opr.pop()
    if opr == '.':
        stack_ch.append(Concat(left, right))
    elif opr == '|':
        stack_ch.append(Union(left, right))