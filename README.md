# truth_table_generator
Hardware is tedious(avoiding to say suck!!!), always has been.

## compiling 

```shellscript
g++ truth_table_generator.cpp -o filename.exe
```

## how to use
```shellscript
./truth_table_generator -f input.txt -s output.txt
```

-f input.txt -> your input is in input.txt

-s output.txt -> your output will be in output.txt

-M -> canonical max term

-m -> canonical min term

--espresso -> make output for espresso (.pla file)

### *Your output file should be .pla if --espresso is added.
### **Note that the program will take canonical minterm if there is no -m or -M inserted.
### ***If you haven't specified an output, the result will be saved as out.txt"

## writing input file

### input.txt
#### F(A, B, C) = minterm(0, 2, 5, 7)
```
A B C
0 2 5 7
```

then call

```shellscript
./truth_table_generator -m -f input.txt -s output.txt
```
