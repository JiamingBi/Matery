class class1{
    var i:int;
    def new(a:int){
        this.i=a;
    }
    def print(){
        output("class1 i=",this.i,"\n");
    }
}

class class2<T>{
    var i:T;
    def new(a:T){
        this.i=a;
    }
    def print(){
        output("class2 i=",this.i,"\n");
    }
}
def main(){
    var c1:class1=class1(10);
    c1.print();
    var c2:class2=class2`int`(20);
    c2.print();
    var c3:class2=class2`float`(30.5);
    c3.print();
}