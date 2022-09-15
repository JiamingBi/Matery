def main(){
    var x:int;
    output("测试终端输入:");
    input(x);
    output("测试终端输出:");
    output("x:",x,"\n");
    var y:string;
    output("测试文件输入:");
    y=readfile("test.mt");
    output(y);
    output("测试文件输出:\n");
    y="djadjiaddddddddddddddddddddddddd";
    writefile("test.txt",y);

}