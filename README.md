# Sistema de Manutenção de Contas em C

Este projeto é um sistema de gerenciamento de clientes utilizando arquivos binários de tamanho fixo em C.

## Recursos Utilizados
- `fseek()` para acesso aleatório às posições do arquivo.
- `fread()` e `fwrite()` para leitura e escrita de registros.
- `rewind()` para retornar o cursor ao início do arquivo.

## Como rodar
Execute no terminal:
gcc main.c -o sistema
./sistema
