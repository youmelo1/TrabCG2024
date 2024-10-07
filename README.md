# TrabGA-CG2024

**Nome**: Rodrigo Fuelber Franke

O objetivo deste trabalho é desenvolver um leitor e visualizador de cenas tridimensionais aplicando os conceitos vistos em aula.  
Ele utiliza o modelo de iluminação Phong para calcular reflexos ambientes, difusos e especulares em objetos carregados a partir de arquivos `.obj`.

Código base fornecido por Rossana Baptista Queiroz ([https://github.com/fellowsheep/CG2024-2](https://github.com/fellowsheep/CG2024-2))

## Controles

### Seleção de Objetos:
- `1, 2, ... 9` – Seleciona um dos objetos carregados para aplicar transformações. O número corresponde à ordem na qual o objeto foi adicionado ao array, onde `1` seleciona o primeiro objeto carregado, `2` o segundo, e assim por diante.

### Movimentação dos Objetos:
- `Setas Esquerda/Direita` – Move o objeto selecionado ao longo do eixo X (esquerda/direita).
- `Setas Cima/Baixo` – Move o objeto selecionado ao longo do eixo Y (cima/baixo).
- `Page Up/Page Down` – Move o objeto selecionado ao longo do eixo Z (frente/trás).
  
> A movimentação só afeta o objeto atualmente selecionado, mantendo os outros objetos em suas posições.
> A velocidade de movimentação é controlada por uma variável que pode ser ajustada para aumentar ou diminuir a velocidade de deslocamento (`objetoSpeed`).

### Movimento da Câmera:
- `W`, `A`, `S`, `D` – Movimenta a câmera para frente, esquerda, trás e direita, respectivamente.
- `Espaço` – Sobe a câmera.
- `Ctrl Esquerdo` – Desce a câmera.

> A velocidade de movimentação da câmera é controlada por uma variável (`cameraSpeed`) que pode ser ajustada para aumentar ou diminuir a velocidade de movimentação.

### Escala do Objeto:
- `=` – Aumenta a escala do objeto.
- `-` – Diminui a escala do objeto.

> A variação na escala é controlada por uma variável (`escalaSize`), que pode ser ajustada para aumentar ou diminuir a quantidade de aumento ou redução da escala do objeto.

### Rotação dos Objetos:
- `X`, `Y`, `Z` – Gira o objeto selecionado ao longo dos eixos X, Y ou Z.
- `R` – Para a rotação do objeto.

> Os objetos podem ser girados continuamente ao longo de qualquer dos eixos. A velocidade de rotação é controlada pelo tempo de execução (`glfwGetTime()`), e o eixo de rotação pode ser alterado pressionando as teclas correspondentes.

