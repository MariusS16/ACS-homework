from .DFA import DFA

from dataclasses import dataclass
from collections.abc import Callable

EPSILON = ''  # this is how epsilon is represented by the checker in the transition function of NFAs


@dataclass
class NFA[STATE]:
    S: set[str]
    K: set[STATE]
    q0: STATE
    d: dict[tuple[STATE, str], set[STATE]]
    F: set[STATE]

    def epsilon_closure(self, state: STATE) -> set[STATE]:
        # compute the epsilon closure of a state (you will need this for subset construction)
        # see the EPSILON definition at the top of this file
        closure = {state}
        stack = [state]
        while stack:
            current_state = stack.pop()
            for next_state in self.d.get((current_state, EPSILON), []):
                # Daca starea urmatoare nu a fost deja vizitata o adaug
                if next_state not in closure:
                    closure.add(next_state)
                    stack.append(next_state)

        return closure

    def subset_construction(self) -> DFA[frozenset[STATE]]:
        states = set()
        d_transitions = {}
        start_q0 = frozenset(self.epsilon_closure(self.q0))
        final_states = set()

        # O stiva pentru a retine starile pe care le voi procesa
        stack = [start_q0]

        for state in start_q0:
            if state in self.F:
                final_states.add(start_q0)
                break

        while stack:
            cur_state = stack.pop()
            states.add(cur_state)

            # Verific daca starea curenta este finala
            for state in cur_state:
                if state in self.F:
                    final_states.add(cur_state)
                    break

            # Pentru fiecare simbol din alfabet, calculez starea urmatoare
            k = len(self.S)
            for i in range(k):
                symbol = list(self.S)[i]
                next_state = set()
                for nfa_state in cur_state:
                    if (nfa_state, symbol) in self.d:
                        for state in self.d[(nfa_state, symbol)]:
                            next_state.update(self.epsilon_closure(state))
                next_state = frozenset(next_state)

                if next_state not in states:
                    stack.append(next_state)
                d_transitions[(cur_state, symbol)] = next_state

        return DFA(
            S=self.S,
            K=states,
            q0=start_q0,
            d=d_transitions,
            F=final_states
        )

    def remap_states[OTHER_STATE](self, f: 'Callable[[STATE], OTHER_STATE]') -> 'NFA[OTHER_STATE]':
        # optional, but may be useful for the second stage of the project. Works similarly to 'remap_states'
        # from the DFA class. See the comments there for more details.
        new_states = {f(state) for state in self.K}
        new_start_state = f(self.q0)
        new_final_states = {f(state) for state in self.F}
        new_transitions = {}

        for (state, symbol), next_states in self.d.items():
            new_state = f(state)
            new_next_states = {f(next_state) for next_state in next_states}
            new_transitions[(new_state, symbol)] = new_next_states

        return NFA(
            S=self.S,
            K=new_states,
            q0=new_start_state,
            d=new_transitions,
            F=new_final_states
        )
