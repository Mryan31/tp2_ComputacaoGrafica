#ifndef FINISH_HPP
#define FINISH_HPP

struct Finish {
    double ka;      // Ambiente
    double kd;      // Difusa
    double ks;      // Especular
    double alpha;   // Brilho (expoente de Phong)
    
    double kr;      // Reflexão
    double kt;      // Transmissão
    double ior;     // Índice de refração (n1/n2)
};

#endif