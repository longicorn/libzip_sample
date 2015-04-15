# libzip sample

on Mac

install libzip
```
$ brew install libzip
```

a.zip's password is 'abc'

```
$ gcc -I/usr/local/Cellar/libzip/<version>/lib/libzip/include/ -o unzip unzip.c -lzip
$ ./unzip
```
