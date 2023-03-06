g++ main.cpp person.cpp pandemic.cpp pandemicCM.cpp
g++ aRoot.cpp -o analysis.out `root-config --cflags --glibs`
g++ aRootCM.cpp -o analysisCM.out `root-config --cflags --glibs`
./a.out
./analysis.out
./analysisCM.out
