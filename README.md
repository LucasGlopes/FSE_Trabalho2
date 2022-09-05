# Fundamentos de Sistemas Embarcados - Trabalho 2 (2022/1)

Este trabalho tem por objetivo a implementação de um sistema (que simula) o controle de um Air Fryer com controle digital.
A descrição completa da atividade pode ser vista [aqui](https://gitlab.com/fse_fga/trabalhos-2022_1/trabalho-2-2022-1).

## Execução do projeto

*  Primeiramente, acesse o diretório do projeto:
```
cd FSE_Trabalho2
```
* Execute o seguinte comando para compilar o projeto:
```
make
```
* Execute o seguinte comando para executar o projeto:
```
bin/run
```

## Uso
 O usuário poderá escolher entre 2 opções:
* Pelo terminal, aonde poderá definir o tempo para aquecimento, com a Temperatura de Referência pela dashboard.
* Por meio de um menu com alimentos pré-selecionados com tempo e temperatura já definidos.

Após o uso, um arquivo log no formato csv será gerado, contendo informações sobre data e hora, temperatura interna, temperatura externa, temperatura definida pelo usuário e valor de acionamento dos atuadores.

