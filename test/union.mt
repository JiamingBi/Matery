union u{
    struct s1 {}
    struct s2 {
        var a : float;
        var b : int;
    }
}

def print(o : u) {
    match (o) {
        s1 {
            output("s1\n");
        }
        s2(s) {
            output("s2.a:", s.a);
            output("\n");
            output("s2.b:", s.b);
            output("\n");
        }
    }
}

def main() {
    var u1 : u;
    u1  = u.s1();
    print(u1);
    u1 = u.s2(1.0,2);
    print(u1);
}
