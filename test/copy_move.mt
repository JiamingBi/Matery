class Bag {
    var ival : int;
    var fval : float;

    def new(a : int, b : float){
        this.ival = a;
        this.fval = b;
    }

    def add() {
        this.ival = this.ival + 1;
        this.fval = this.fval + 1.0;
    }

    def output() {
        output(this.ival, this.fval);
        output("\n");
    }
}

def move() {
    output("Display `move` behavior:");
    output("\n");
    var bag : Bag = Bag(1, 2.0);
    var bag2 : Bag;
    bag2 = bag;  # ownership transfered: bag --> bag2
    bag2.add();
    debug(bag);  # debug info shows that bag refers to nothing
    bag2.output();
}

def copy() {
    output("Display `copy` behavior:");
    output("\n");
    var bag : Bag = Bag(1, 2.0);
    var bag2 : Bag;
    bag2 := bag;  # bag and bag2 points to the same object
    bag2.add();   # both bag and bag2 are modified
    bag.output();
    bag2.output();
}

def main() {
    move();
    copy();
}
