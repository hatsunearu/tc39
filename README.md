# TC39

TC39 (Toy Cipher 39) is a simple encryption software using the Speck cipher made by the NSA for embedded software systems. This is made for my own amusement and education implementing cryptosystems.

Warning: There are ABSOLUTELY NO GUARANTEES about the security of this software. While I tried my best, there may be huge security flaws. Do NOT use this software for communicating stuff that you don't want others to see.

## Compilation

```
make clean
make
```

## Examples

Encode then decode
```
cat myname
./tc39 -e -i myname -o myname.ct
hexdump -Cv myname.ct
./tc39 -d -i myname.ct -o myname.out
cat myname.out
```
