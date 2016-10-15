#Sulabha Samskrita Tankana Lekhani
#सुलभ संस्कृतटङ्कणलेखनी

##For Linux
How to use
==========

1. Open a terminal.
2. Locate this(current) directory.
3.	  `chmod +x SSTL.run configure.sh`
4.	 `./configure.sh`   (For Ubuntu and Debian based systems)
(All the libraries are inbuilt in Ubuntu, Linux Mint)

5. 	 `./SSTL.run`
6. Press F8 to disable SSTL. Again press F8 to enable.
7. Now you can type anywhere in Devanagari.
8. Goto the terminal and press Ctrl+c to quit the application.

Tested On
=========

BOSS Linux 5.1  (64 bit)   (Bharat Operating System Solutions)[ https://bosslinux.in ]
Ubuntu 14.04    (64 bit)
Linux Mint 17.1 (32 bit)
Fedora 23       (64 bit)

How to develop
==============

1. The src folder contains all the sources required to build this program.
2. 	`gcc main.c -lX11 -lXtst`		to compile from source
3. If you find any bugs or would like to contribute then you can visit
	
	[ https://github.com/chaitanya-lakkundi/samskrit-keyboard ]

Examples:

1. mm nAm chaitanfyH / mama naama chaitanfyaH
        मम नाम चैतन्यः    
2. kRRidanfta ruupa mAlA / kRRidnft rUp maalaa
        कृदन्त रूप माला

Please note that character f is used for halanta. To type combined words a halanta needs to be added at the end of first character.
Eg. To type विश्व   --   vishfva        (notice that f is typed after sh which adds a halanta to श and make it श् )

ka      क
kaf     क्

##Full Keyboard Map

Vowels (dependent and independent):
-----------------------------------

a     aa / A       i      ii / I       u     uu / U 
RRi / R^i    RRI / R^I    LLi / L^i    LLI / L^I
e     ai     o     au     aM    aH

Consonants:
----------- 
k     kh     g     gh     ~N
ch    Ch     j     jh     ~n
T     Th     D     Dh     N
t     th     d     dh     n
p     ph     b     bh     m
y     r      l     v / w
sh    Sh     s     h      L
j~n

Specials/Accents:
-----------------
Anusvara:       .n / M / .m  (dot on top of previous consonant/vowel)
Avagraha:       .a    ('S' like symbol basically to replace a after o)
Ardhachandra:   .c    (for vowel sound as in english words 'cat' or 'talk')
Chandra-Bindu:  .N    (chandra-bindu on top of previous letter)
Halant:		    .h    (to get half-form of the consonant - no vowel - virama)
Visarga:        H     (visarga - looks like a colon character)
Om:		        OM, AUM (Om symbol)


[As shown, many codes have multiple choices, example "RRi / R^i" implies you
 can use either "RRi" or "R^i"]
 
## **धन्यवादाः**

