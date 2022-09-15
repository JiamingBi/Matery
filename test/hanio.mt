def main()
{
    
    var m:int;
     m=4;
     hanoi(m,'A','B','C');
}
def hanoi(n:int,one:char,two:char,three:char)
{
     if (n == 1)
     {
           move(one,three);
     }
     else
     {
         hanoi(n-1,one,three,two);
         move(one,three);
         hanoi(n-1,two,one,three);
     }
}

def move(x:char, y:char)
{
     output(x,"-->",y,"\n");
}