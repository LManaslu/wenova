# Wenova: Rise of Conquerors

## Sumário
* [1. Descrição do Jogo](#1-descrição-do-jogo)  
* [2. Informações Gerais](#2-informações-gerais)
* [2.1. História](#21-história)
* [2.2. Screenshots](#22-screenshots)
* [2.3. Gameplay](#23-gameplay)   
* [3. Jogabilidade](#3-jogabilidade)
* [3.1. Objetivos](#31-objetivos)
* [3.2. Controles](#32-controles)     
* [4. Outras Informações](#4-outras-informações)
* [4.1. Dependências](#41-dependências)   
* [4.2. Instalação e Compilação](#42-instalação-e-compilação)
* [4.3. Licença](#43-licença)
* [5. Time](#5-time)   

------------------------------------------------------------

## 1. Descrição do Jogo


## 2. Informações Gerais


### 2.1. História


### 2.2. Screenshots


### 2.3. Gameplay


## 3. Jogabilidade


### 3.1. Objetivos


### 3.2. Controles

As ações dos 4 jogadores durante o jogo serão controladas por joysticks. Como alternativa, é possível substituir qualquer um dos joysticks pelo teclado.

As ações principais do jogador estão listadas abaixo:

| Contexto | Ações |
| :------: | --- |
| Menus | <div style="text-align: left;"><div style="display: inline-block; text-align: left"><li>Direcionais</li><li>Selecionar</li><li>Cancelar</li><li>Bloquear</li></div></div> |
| Batalha | <div style="text-align: left;"><div style="display: inline-block; text-align: left"><li>Movimentação</li><li>Pular</li><li>Agachar</li><li>Ataque base</li><li>Ataques direcionais</li><li>Ataques direcionais no pulo</li><li>Ataques direcionais ao agachar</li><li>Habilidade 1</li><li>Habilidade 2</li><li>Ataque Especial</li></div></div> |

Abaixo se encontram os mapeamentos das teclas dos joysticks e do teclado respectivamente (em inglês):

![joystick](res/readme/joystick.png)

![keyboard](res/readme/keyboard.png)

## 4. Outras Informações
### 4.1. Dependências

* **C++**

`c++`

* **SDL 2.0**

`libsdl2-2.0-0 libsdl2-dev`

* **SDL Image 2.0**

`libsdl2-image-2.0-0 libsdl2-image-dev`

* **SDL Mixer 2.0**

`libsdl2-mixer-2.0-0 libsdl2-mixer-dev`

* **SDL TTF 2.0**

`libsdl2-ttf-2.0-0 libsdl2-ttf-dev`

### 4.2. Instalação e Compilação

* **Instalar**

__Passo 1:__ Clone o repositório

`$ git clone https://github.com/LManaslu/Wenova.git`

__Passo 2:__ Instale o compilador de C++

`$ sudo apt-get install g++`

__Passo 3:__ Instale as bibliotecas SDL

`$ sudo apt-get install libsdl2-2.0-0 libsdl2-dev libsdl2-image-2.0-0 libsdl2-image-dev libsdl2-ttf-2.0-0 libsdl2-ttf-dev`

* **Compilar**

Para compilar todos os arquivos .cpp da pasta src/ e todos os cabeçalhos .h da pasta include/, use o comando `make`:

`$ make`

Opcionalmente, para uma compilação mais rápida, você pode usar a flag -j:

`$ make -j`


* **Limpar pasta obj/**

Para remover todos os arquivos .d e .o da pasta obj/ , utilize o comando clean:

`$ make clean`

* **Limpar pasta obj/ e deletar executável do jogo**

Para remover todos os arquivos .d e .o da pasta obj/ e excluir o binário executável da pasta bin/, utilize o comando **dist-clean**:

`$ make dist-clean`

* **Execução do Jogo**

Para rodar o jogo compilado, utilize o comando `run`:

`$ make run`

### 4.3. Licença

A licença deste jogo pode ser visualizada [aqui](LICENSE).

## 5. Time

A equipe responsável pelo desenvolvimento do jogo ***Wenova - Rise of Conquerors*** é formada por:

### Desenvolvimento 
* Igor Ribeiro Barbosa Duarte
* João Paulo Busche da Cruz
* João Vítor Araujo Moura
* Vítor Barbosa de Araujo

### Arte
* Francisco Sales de Melo Matias
* Rodrigo Maia Dal Moro

### Música
* Enzo Aprile Lopes Velloso
