# Java compiler

Simple compiler for the subset of Java programming language for Linux.

*Uses abstract syntax tree & visitor pattern for parsing.*

### Supported features: 
* comparisons (==, >, <, >=, <=, !=);
* inc and dec (++, --)
* arithmetic expressions (-, +, /, %);
* logical expressions (!, &&, ||);
* boolean and int variables;
* while and if constructions
* printing strings (System.out.println("line")
* blocks ({})

Warning: variables are not allowed to be initialized.

Program structure looks like:

```
public class ClassName
{
    public static void main() {
        // smth you want, for example:
        int i;
        i = 0;
        while (i++ < 5)
            System.out.println("PRIVET");
    }
}
```
## Compiling
First you need to generate Makefile:
```
$ cmake CMakeLists.txt
```
Then you can compile program with `make all`.

## Options for the program
```
$ ./compiler filename [-o file] [-t] [-p] [-c]
```
* **-o** determines filename for assembler code to be written to. By default file  `out` is used.
* **-t** tells compiler to make only lexical analysis and to write all found lexemes to stdout.
* **-p** tells compiler to make also synthaxic analysis (nothing is printed).
* **-c** tells compiler not to generate assembler code.

## Running assembler code
For running you should use **cvm** virtual machine by A. Chernov (see cvm/README.md for info).
You can run your code by script `run.sh` (assuming the code is in file named `out`).
Anyway, you can also run your code by typing:
```
$ ./cvm/cvmas filename >> temp
$ ./cvm/cvm temp
$ rm temp
```
where `filename` is the name of file with generated assembler code.
