Simple tracking program. It uses a (any size) chessboard, a webcam and sends the chessboard position via network (UDP protocol).

*Installation*
You need QT(>=4) and OpenCV libraries, plus qmake and make buiding tools.

Copy this repository

```
git clone https://github.com/kwahoo2/unitracker
```

Adjust library paths in the Unitracker.pro file if necessary.
Build and run the application.

```
cd unitracker
qmake
make
./Unitracker
```

*Transformation matrix*
4x4 Transformation matrix is transmitted as 16 doubles using the UDP protocol. 

```
Matrix =
((r11,r12,r13,r11*X+r12*U+r13*Z),
(r21,r22,r23,r21*X+r22*Y+r23*Z),
(r31,r32,r33,r31*X+r32*Y+r33*Z),
(0,0,0,1)) 
```
