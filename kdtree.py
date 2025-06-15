import random
from typing import List, Optional, Union, Set

def format_num(num:int|float):
    if isinstance(num, int):
        return str(num)
    else:
        s = f'{num:.2f}'
        if s.startswith('0.'):
            s = s.removeprefix('0')
        return s

def format_vec(vec:list[int|float]):
    return '[' + ', '.join(format_num(x) for x in vec) + ']'

class RKDNode:
    def __init__(self, key: List[float], discr: int, left=None, right=None):
        self.key = key
        self.discr = discr
        self.left: Optional[RKDNode] = left
        self.right: Optional[RKDNode] = right
        self.size = 1 + (left.size if left else 0) + (right.size if right else 0)
        self.dim = len(key)

    def print(self, prefix_root="", prefix_left="", prefix_right=""):
        indent = self.dim * 5 + 4
        bar_space = " |" + " " * (indent - 2)
        space = " " * (indent)
        line = "-" * (indent - 2)
        if self.left:
            self.left.print(prefix_left + " ." + line, prefix_left + space, prefix_left + bar_space)
        print(f"{prefix_root}({self.discr}) {format_vec(self.key)}")
        if self.right:
            self.right.print(prefix_right + " '" + line, prefix_right + bar_space, prefix_right + space)



def update_size(node: Optional[RKDNode]):
    if node:
        node.size = 1 + (node.left.size if node.left else 0) + (node.right.size if node.right else 0)

def split(T: Optional[RKDNode], x: List[float], i: int) -> tuple[Optional[RKDNode], Optional[RKDNode]]:
    if not T:
        return None, None

    j = T.discr
    if i == j:
        if x[i] < T.key[i]:
            L, R = split(T.left, x, i)
            T.left = R
            update_size(T)
            return L, T
        else:
            L, R = split(T.right, x, i)
            T.right = L
            update_size(T)
            return T, R
    else:
        L1, R1 = split(T.left, x, i)
        L2, R2 = split(T.right, x, i)
        if x[i] < T.key[i]:
            left = join(L1, L2, j)
            T.left, T.right = R1, R2
            update_size(T)
            return left, T
        else:
            right = join(R1, R2, j)
            T.left, T.right = L1, L2
            update_size(T)
            return T, right

def join(U: Optional[RKDNode], V: Optional[RKDNode], i: int) -> Optional[RKDNode]:
    if not U:
        return V
    if not V:
        return U

    m, n = U.size, V.size
    if random.randint(0, m + n - 1) < m:
        j = U.discr
        if i == j:
            U.right = join(U.right, V, i)
            update_size(U)
            return U
        else:
            L, R = split(V, U.key, j)
            U.left = join(U.left, L, i)
            U.right = join(U.right, R, i)
            update_size(U)
            return U
    else:
        j = V.discr
        if i == j:
            V.left = join(U, V.left, i)
            update_size(V)
            return V
        else:
            L, R = split(U, V.key, j)
            V.left = join(L, V.left, i)
            V.right = join(R, V.right, i)
            update_size(V)
            return V

def insert_at_root(T: Optional[RKDNode], x: List[float]) -> RKDNode:
    i = random.randint(0, len(x) - 1)
    L, R = split(T, x, i)
    new_node = RKDNode(key=x, discr=i, left=L, right=R)
    update_size(new_node)
    return new_node

def insert(T: Optional[RKDNode], x: List[float]) -> RKDNode:
    n = T.size if T else 0
    if random.randint(0, n) == n:
        return insert_at_root(T, x)
    else:
        assert T is not None
        i = T.discr
        if x[i] < T.key[i]:
            T.left = insert(T.left, x)
        else:
            T.right = insert(T.right, x)
        update_size(T)
        return T

def delete_root(T: Optional[RKDNode]) -> Optional[RKDNode]:
    if not T:
        return None
    return join(T.left, T.right, T.discr)

def delete(T: Optional[RKDNode], x: List[float]) -> Optional[RKDNode]:
    if not T:
        return None
    i = T.discr
    if T.key == x:
        return delete_root(T)
    if x[i] < T.key[i]:
        T.left = delete(T.left, x)
    else:
        T.right = delete(T.right, x)
    update_size(T)
    return T

class RelaxedKDTree:
    def __init__(self, points: Optional[List[List[float]]] = None):
        self.root: Optional[RKDNode] = None
        if points:
            for p in points:
                self.insert(p)

    def insert(self, x: List[float]):
        self.root = insert(self.root, x)

    def delete(self, x: List[float]):
        self.root = delete(self.root, x)

    def partial_match(self, query: List[Union[float, str]]) -> tuple[int, int]:
        def _pm(node:RKDNode):
            nonlocal visits, matches
            if node is None:
                return
            visits += 1
            if all(q == '?' or q == v for q, v in zip(query, node.key)):
                matches += 1
            if node.left and (
                query[node.discr] == '?' or 
                query[node.discr] == node.key[node.discr]
            ):
                _pm(node.left)
            if node.right and (
                query[node.discr] == '?' or 
                query[node.discr] == node.key[node.discr]
            ):
                _pm(node.right)

        visits, matches = 0, 0
        _pm(self.root)
        return visits, matches

    def print_tree(self):
        self.root.print()

if __name__ == '__main__':
    tree = RelaxedKDTree()
    for _ in range(100):
        vec = [random.uniform(0, 1) for _ in range(2)]
        tree.insert(vec)
        if _ % 5 == 5-1:
            tree.print_tree()
            input('Press ENTER to continue')