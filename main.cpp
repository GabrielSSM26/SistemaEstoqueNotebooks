#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

struct Notebook {
    int codigo;
    char marca[30];
    char modelo[40];
    char processador[40];
    int memoriaRAM;
    char armazenamento[30];
    float preco;
    int quantidade;
    int status;
};

bool codigoExiste(int codigo);

void cadastrarNotebook() {

    FILE *arquivo;
    Notebook notebook;

    cout << "\n========== CADASTRO DE NOTEBOOK ==========";

    cout << "\nDigite o codigo: ";
    cin >> notebook.codigo;

    if (codigoExiste(notebook.codigo)) {
        cout << "\nEsse codigo ja esta cadastrado!";
        return;
    }

    cin.ignore();

    cout << "Digite a marca: ";
    cin.getline(notebook.marca, 30);

    cout << "Digite o modelo: ";
    cin.getline(notebook.modelo, 40);

    cout << "Digite o processador: ";
    cin.getline(notebook.processador, 40);

    cout << "Digite a memoria RAM (GB): ";
    cin >> notebook.memoriaRAM;

    cin.ignore();

    cout << "Digite o armazenamento: ";
    cin.getline(notebook.armazenamento, 30);

    cout << "Digite o preco: ";
    cin >> notebook.preco;

    cout << "Digite a quantidade: ";
    cin >> notebook.quantidade;

    notebook.status = 1;

    arquivo = fopen("estoque.dat", "ab");

    if (arquivo == NULL) {
        cout << "\nErro ao abrir o arquivo!";
        return;
    }

    fwrite(&notebook, sizeof(Notebook), 1, arquivo);

    fclose(arquivo);

    cout << "\nNotebook cadastrado com sucesso!";
}

void consultarNotebook() {

    FILE *arquivo;
    Notebook notebook;
    int codigo;
    bool encontrado = false;

    cout << "\nDigite o codigo do notebook: ";
    cin >> codigo;

    arquivo = fopen("estoque.dat", "rb");

    if (arquivo == NULL) {
        cout << "\nNenhum notebook cadastrado!";
        return;
    }

    while (fread(&notebook, sizeof(Notebook), 1, arquivo) == 1) {

        if (notebook.codigo == codigo && notebook.status == 1) {

            cout << "\n====== NOTEBOOK ENCONTRADO ======";
            cout << "\nCodigo: " << notebook.codigo;
            cout << "\nMarca: " << notebook.marca;
            cout << "\nModelo: " << notebook.modelo;
            cout << "\nProcessador: " << notebook.processador;
            cout << "\nMemoria RAM: " << notebook.memoriaRAM << " GB";
            cout << "\nArmazenamento: " << notebook.armazenamento;
            cout << "\nPreco: R$ " << notebook.preco;
            cout << "\nQuantidade: " << notebook.quantidade;

            encontrado = true;
            break;
        }
    }

    fclose(arquivo);

    if (!encontrado) {
        cout << "\nNotebook nao encontrado!";
    }
}

void listarNotebooks() {

    FILE *arquivo;
    Notebook notebook;
    int contador = 0;

    arquivo = fopen("estoque.dat", "rb");

    if (arquivo == NULL) {
        cout << "\nNenhum notebook cadastrado!";
        return;
    }

    cout << "\n======= NOTEBOOKS CADASTRADOS =======";

    while (fread(&notebook, sizeof(Notebook), 1, arquivo) == 1) {

        if (notebook.status == 1) {

            cout << "\n\nCodigo: " << notebook.codigo;
            cout << "\nMarca: " << notebook.marca;
            cout << "\nModelo: " << notebook.modelo;
            cout << "\nProcessador: " << notebook.processador;
            cout << "\nMemoria RAM: " << notebook.memoriaRAM << " GB";
            cout << "\nArmazenamento: " << notebook.armazenamento;
            cout << "\nPreco: R$ " << notebook.preco;
            cout << "\nQuantidade: " << notebook.quantidade;

            cout << "\n-----------------------------------";

            contador++;
        }
    }

    fclose(arquivo);

    if (contador == 0) {
        cout << "\nNenhum notebook ativo encontrado!";
    }
}

void excluirNotebook() {

    FILE *arquivo;
    Notebook notebook;
    int codigo;
    bool encontrado = false;
    long posicao;

    cout << "\nDigite o codigo do notebook que deseja excluir: ";
    cin >> codigo;

    arquivo = fopen("estoque.dat", "rb+");

    if (arquivo == NULL) {
        cout << "\nErro ao abrir o arquivo!";
        return;
    }

    while (true) {

        posicao = ftell(arquivo);

        if (fread(&notebook, sizeof(Notebook), 1, arquivo) != 1) {
            break;
        }

        if (notebook.codigo == codigo && notebook.status == 1) {

            notebook.status = 0;

            fseek(arquivo, posicao, SEEK_SET);

            fwrite(&notebook, sizeof(Notebook), 1, arquivo);

            encontrado = true;
            break;
        }
    }

    fclose(arquivo);

    if (encontrado) {
        cout << "\nNotebook excluido com sucesso!";
    }
    else {
        cout << "\nNotebook nao encontrado ou ja excluido!";
    }
}

void exportarTXT() {

    FILE *arquivoBinario;
    FILE *arquivoTXT;
    Notebook notebook;
    int contador = 0;

    arquivoBinario = fopen("estoque.dat", "rb");

    if (arquivoBinario == NULL) {
        cout << "\nNenhum notebook cadastrado!";
        return;
    }

    arquivoTXT = fopen("estoque.txt", "w");

    if (arquivoTXT == NULL) {
        cout << "\nErro ao criar arquivo TXT!";
        fclose(arquivoBinario);
        return;
    }

    fprintf(arquivoTXT, "========== ESTOQUE DE NOTEBOOKS ==========\n\n");

    while (fread(&notebook, sizeof(Notebook), 1, arquivoBinario) == 1) {

        if (notebook.status == 1) {

            fprintf(arquivoTXT, "Codigo: %d\n", notebook.codigo);
            fprintf(arquivoTXT, "Marca: %s\n", notebook.marca);
            fprintf(arquivoTXT, "Modelo: %s\n", notebook.modelo);
            fprintf(arquivoTXT, "Processador: %s\n", notebook.processador);
            fprintf(arquivoTXT, "Memoria RAM: %d GB\n", notebook.memoriaRAM);
            fprintf(arquivoTXT, "Armazenamento: %s\n", notebook.armazenamento);
            fprintf(arquivoTXT, "Preco: R$ %.2f\n", notebook.preco);
            fprintf(arquivoTXT, "Quantidade: %d\n", notebook.quantidade);

            fprintf(arquivoTXT, "\n------------------------------------------\n\n");

            contador++;
        }
    }

    if (contador == 0) {
        fprintf(arquivoTXT, "Nenhum notebook ativo cadastrado.\n");
    }

    fclose(arquivoBinario);
    fclose(arquivoTXT);

    cout << "\nEstoque exportado com sucesso!";
    cout << "\nQuantidade de notebooks exportados: " << contador;
}

void contabilizarNotebooks() {

    FILE *arquivo;
    Notebook notebook;
    int ativos = 0;
    int totalRegistros = 0;

    arquivo = fopen("estoque.dat", "rb");

    if (arquivo == NULL) {
        cout << "\nNenhum notebook cadastrado!";
        return;
    }

    while (fread(&notebook, sizeof(Notebook), 1, arquivo) == 1) {

        totalRegistros++;

        if (notebook.status == 1) {
            ativos++;
        }
    }

    fclose(arquivo);

    cout << "\n======= CONTROLE DE ESTOQUE =======";
    cout << "\nTotal de registros armazenados: " << totalRegistros;
    cout << "\nTotal de notebooks ativos: " << ativos;
    cout << "\nTotal de notebooks excluidos: " << totalRegistros - ativos;
}

bool codigoExiste(int codigo) {

    FILE *arquivo;
    Notebook notebook;

    arquivo = fopen("estoque.dat", "rb");

    if (arquivo == NULL) {
        return false;
    }

    while (fread(&notebook, sizeof(Notebook), 1, arquivo) == 1) {

        if (notebook.codigo == codigo && notebook.status == 1) {
            fclose(arquivo);
        return true;
        }
    }

    fclose(arquivo);

    return false;
}

int main() {

    int opcao;

    do {
        cout << endl;
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
        cout << "\n        SISTEMA DE ESTOQUE";
        cout << "\n            NOTEBOOKS";
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
        cout << "\n1 - Cadastrar notebook";
        cout << "\n2 - Consultar notebook";
        cout << "\n3 - Listar notebooks";
        cout << "\n4 - Excluir notebook";
        cout << "\n5 - Exportar estoque para TXT";
        cout << "\n6 - Contabilizar notebooks";
        cout << "\n0 - Sair";
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
        cout << "\nEscolha uma opcao: ";

        cin >> opcao;

        switch (opcao) {

            case 1:
                cadastrarNotebook();
                break;

            case 2:
                consultarNotebook();
                break;

            case 3:
                listarNotebooks();
                break;

            case 4:
                excluirNotebook();
                break;

            case 5:
                exportarTXT();
                break;

            case 6:
                contabilizarNotebooks();
                break;

            case 0:
                cout << "\nEncerrando o sistema...";
                break;

            default:
                cout << "\nOpcao invalida!";
        }

    } while (opcao != 0);

    return 0;
}