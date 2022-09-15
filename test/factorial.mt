def main() {
    var i : int;
    const len = 11;
    const j = "Printing factorial from 0 to \n";
    var arr : int[11];
    output(j, len - 1);
    output("\n");
    i = 0;
    while (i < len ) {
        arr[i] = fact(i);
        i = i + 1;
    }

    for (i = 0; i < len; i = i + 1) {
        output(i, "=", arr[i]);
        output("\n");
    }
    var k :int;
    k=i;
    output(i);
    output("\n");
    output(k);
    output("\n");
    #fact2(i);
}

def fact(a : int) -> int {
    if (a > 0) {
        return a * fact(a - 1);
    } else {
        return 1;
    }
}

def fact2(a : int)  {
    var i :int;
    i:=a;
    print(i);
    print(a);

}
