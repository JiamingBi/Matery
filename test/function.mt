def fun1(a:int){
    output(a,"\n");
}

def fun2<T>(b:T){
    output(b,"\n");
}

def main(){
    var i:int;
    i=1;
    fun1(i);
    var j:int;
    j=2;
    fun2`int`(j);
    var k:float;
    k=2.2;
    fun2`float`(k);
}