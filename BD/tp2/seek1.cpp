#include "primaria.hpp"
#include "hash.hpp"
#include <iostream>
#include <fstream>

int blocks_read = 0;

Article findRecordByPosition(BplusTree bptree, int id, const std::string& bucket_filename, const std::string& overflow_filename) {
    std::ifstream file(bucket_filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo principal para leitura!" << std::endl;
        throw std::runtime_error("Erro ao abrir o arquivo principal.");
    }

    // Tentar localizar o registro no arquivo principal
    std::streampos pos = bptree.searchKey(id);  // Posição do registro a ser encontrado
    std::cout << "Posição do registro: " << pos << std::endl;
    blocks_read = 0;  // Zerar o contador de blocos lidos
    file.seekg(pos); // Posiciona-se no bloco correto
    Block current_block;
    file.read(reinterpret_cast<char*>(&current_block), sizeof(Block));
    blocks_read++;  // Incrementa o contador de blocos lidos

    // Realizar a busca no bloco do arquivo principal
    int recordIndex = binarySearchInBlock(current_block, id);
    if (recordIndex != -1) {
        // Registro encontrado no arquivo principal
        cout << "\n---Registro encontrado no arquivo principal.---" << endl;
        return current_block.records[recordIndex];
    }

    // Caso não encontrado, abrir o arquivo de overflow
   
    std::ifstream overflow_file(overflow_filename, std::ios::binary);
    if (!overflow_file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de overflow para leitura!" << std::endl;
        throw std::runtime_error("Erro ao abrir o arquivo de overflow.");
    }

    Article article;
    while (overflow_file.read(reinterpret_cast<char*>(&article), sizeof(Article))) {
        blocks_read++;  // Incrementa o contador de blocos lidos
        if (article.id == id) {
            overflow_file.close();
            cout << "\n---Registro encontrado no arquivo de overflow.---" << endl;
            return article;
        }
    }
    


    // Se o laço termina, significa que não encontrou o registro
    std::cerr << "Registro com ID " << id << " não encontrado." << std::endl;
    throw std::runtime_error("Registro não encontrado.");
}


int main() {
    std::string bucket_filename = "articles.bin";
    std::string overflow_filename = "overflow.bin";
    BplusTree bptree(3); // Ordem da B+ Tree
    int id;  // ID do registro a ser encontrado
    std::cout << "Digite o ID do registro a ser encontrado: ";
    std::cin >> id;
    bptree.loadFromFile("index.bin");

    try {
        // Encontrar o registro na posição especificada
        Article found_article = findRecordByPosition(bptree, id, bucket_filename, overflow_filename);

        // Imprimir os dados do registro encontrado
        std::cout << "ID: " << found_article.id << std::endl;
        std::cout << "Título: " << found_article.title << std::endl;
        std::cout << "Ano: " << found_article.year << std::endl;
        std::cout << "Autores: " << found_article.authors << std::endl;
        std::cout << "Citações: " << found_article.citations << std::endl;
        std::cout << "Data de Atualização: " << found_article.updated_at << std::endl;
        std::cout << "Snippet: " << found_article.snippet << std::endl;

        std::cout << "\nBlocos lidos: " << blocks_read << std::endl;
        std::cout << "Total de blocos no arquivo de indice primario: " << bptree.total_blocks << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Erro ao encontrar o registro: " << e.what() << std::endl;
    }

    return 0;
}
