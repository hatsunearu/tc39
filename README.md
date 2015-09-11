# TC39

TC39 (Toy Cipher 39) is a simple encryption software using the Speck cipher made by the NSA for embedded software systems. This is made for my own amusement and education implementing cryptosystems.

Warning: There are ABSOLUTELY NO GUARANTEES about the security of this software. While I tried my best, there may be huge security flaws. Do NOT use this software for communicating stuff that you don't want others to see.

## How it works

TC39 uses the Speck128/128 as its block cipher primitive. The implementation of Speck is adopted from the [Wikipedia entry of Speck](https://en.wikipedia.org/wiki/Speck_%28cipher%29) which itself is adopted from the [IACR ePrint](http://eprint.iacr.org/2013/404).

TC39 uses Speck128/128 in CTR mode. During encryption, a random 128 bit nonce is generated and put at the beginning of the output file. Then, TC39 processes the input file 128 bits at a time. If the entire file cannot fit in a 128 bit block, it is zero padded. 

Currently the random number generator is a dummy implementation, but the core CTR mode encryption seems to work.

## Todo

* Implement MAC

* Implement Base64 Encode and Decode for processing ciphertext

* Better command line options 

* Implement multithreaded/parallel decode and encode

* Try porting to different languages 

## Compilation

```
make clean
make
```

## Examples

### Generate key
```
dd if=/dev/urandom of=key bs=1 count=16
```

### Encode
```
cat myname
./tc39 -e myname.in -o myname.tc39 -k key
hexdump -Cv myname.tc39
```
### Decode
```
./tc39 -d myname.tc39 -o myname.out -k key
cat myname.out
```

