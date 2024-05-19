#include "centroide.h"
#include <vector>
#include <random>
#include <fstream>
#include <filesystem> 

// Construtor
Centroide::Centroide(int id, vector<double> atributos, vector<Instancia> instancias_proximas)
    : id(id), atributos(atributos), instancias_proximas(instancias_proximas) {}

// Getters
int Centroide::getId() const {
    return id;
}

vector<double> Centroide::getAtributos() const {
    return atributos;
}

vector<Instancia> Centroide::getProximos() const {
    return instancias_proximas;
}

// Setters
void Centroide::setId(int id) {
    this->id = id;
}

void Centroide::setAtributos(const vector<double>& atributos) {
    this->atributos = atributos;
}

void Centroide::setProximos(const vector<Instancia>& proximos) {
    this->instancias_proximas = proximos;
}

Centroide Centroide::criarCentroideAleatorio(int id, vector<Instancia> instancias){
    int numAtributos = instancias[0].getAtributos().size();
    double menor;
    double maior;
    vector<double> atributos;

    for(int i = 0; i < numAtributos; i++){
        double soma = 0.0;
        double variancia = 0.0;
        vector<double> atributosAvaliados;

        for(int j = 0; j < instancias.size(); j++){
            double temp = instancias[j].getAtributos()[i];
            if(j == 0){
                menor = temp;
                maior = temp;
            }
            soma += temp;
            atributosAvaliados.push_back(move(temp));
            if(menor > temp){
                menor = temp;
            } else if (maior < temp){
                maior = temp;
            }
        }

        soma = soma/instancias.size();
        for(double num : atributosAvaliados){
            variancia += (num - soma) * (num - soma);
        }
        variancia = variancia / atributosAvaliados.size();

        double desvioPadrao = sqrt(variancia);

        random_device rd;
        mt19937 gen(rd());
        double media = (menor + maior) / 2.0;
        normal_distribution<double> dis(media, desvioPadrao);

        double temp;
        do{
            temp = dis(gen);
        } while((temp < menor) || (temp > maior));

        atributos.push_back(move(temp));
    }

    vector<Instancia> proximos;
    Centroide centroide(id, atributos, proximos);

    return centroide;
}

void Centroide::escreverCentroide(const vector<Centroide>& centroides, const string& nome_arquivo){
    string pasta = "OutputTeste";
    filesystem::path directory = pasta;

    // Cria a pasta se ela não existir
    if (!filesystem::exists(directory)) {
        if (!filesystem::create_directories(directory)) {
            cerr << "Erro ao criar a pasta: " << directory << endl;
            return;
        }
    }

    filesystem::path caminho_arquivo = directory / nome_arquivo;

    ofstream arquivo(caminho_arquivo);

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo para escrita: " << caminho_arquivo << endl;
        return;
    }

    for (const Centroide& centroide : centroides) {
        arquivo << "Centroide ID: " << centroide.getId() << " - Atributos: ";
        for (double atributo : centroide.getAtributos()) {
            arquivo << fixed << atributo << " ";
        }
        arquivo << endl;
    }

    arquivo.close();
}