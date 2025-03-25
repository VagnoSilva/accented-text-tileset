#include <genesis.h>
#include "resources.h"
#include "text.h"
int bytes_consumed = 0;
// Função para mapear o caractere para o índice da tile no tileset
size_t getCharacterTile(const char *text, int *bytes_consumed) {
    // Mapeamento de caracteres para índices no tileset (valores combinados para UTF-8)
    unsigned int characters[] = {
        ' ', '!', '?', '#', '$', '%', 0xA8, '&', '*', '(', ')', '-', '+', ',', '.', '/',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', 0xC3A1, 0xC3A9, 0xC3AD, 0xC3B3,
        0xC3BA, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
        'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', 0xC387, 0xC3A7,
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
        'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 0xC3A3, 0xC3B5, 0xC3A2, 0xC3AA, 0xC3B4, 0xC3B1
    };

    size_t index = (size_t)-1;

    // Verifica se o caractere é multibyte (primeiro byte começa com 110xxxxx)
    if ((unsigned char)text[0] >> 5 == 0x6) {  // 0xC3 -> 11000011
        // Combina os dois bytes em um valor único
        unsigned int combined = ((unsigned char)text[0] << 8) | (unsigned char)text[1];

        // Busca o índice do caractere na tabela
        for (size_t i = 0; i < sizeof(characters) / sizeof(characters[0]); i++) {
            if (characters[i] == combined) {
                index = i;
                *bytes_consumed = 2;  // Dois bytes consumidos para este caractere
                break;
            }
        }
    } else {
        // Caractere ASCII simples (um byte)
        for (size_t i = 0; i < sizeof(characters) / sizeof(characters[0]); i++) {
            if (characters[i] == (unsigned char)text[0]) {
                index = i;
                *bytes_consumed = 1;  // Um byte consumido para este caractere
                break;
            }
        }
    }

    return index;  // Retorna o índice do caractere no tileset (0 a 95)
}

#define TILESET_OFFSET 0x5A0  // Deslocamento base para o tileset
#define PLANE_BG BG_A         // Define o plano de fundo como BG_A

// Função para desenhar um caractere com seu tileset
void drawCharacter(int x, int y, const char *text, u16 plane) {

    size_t tileIndex = getCharacterTile(text, &bytes_consumed);  // Obtém o índice do caractere no tileset

    if (tileIndex != (size_t)-1) {
        u16 adjustedIndex = TILESET_OFFSET + tileIndex;  // Aplica o deslocamento

        // Renderiza o caractere no plano especificado
        VDP_setTileMapXY(plane, adjustedIndex, x, y);
    } else {
        // Substitui caracteres não mapeados por espaço (' ')
        u16 adjustedIndex = TILESET_OFFSET + 0;  // Índice do espaço no tileset
        VDP_setTileMapXY(plane, adjustedIndex, x, y);
    }
}

// Função para desenhar texto com quebra de linha
void drawText(int x, int y, const char* text) {
    int currentX = x;
    int currentY = y;

    // Itera sobre cada caractere da string
    for (int i = 0; text[i] != '\0';) {
        char c = text[i];

        if (c == '\n') {
            // Quebra de linha
            currentX = x;  // Reinicia a posição X
            currentY++;    // Move para a próxima linha
            i++;
        } else {
            // Desenha o caractere e avança o número correto de bytes
            drawCharacter(currentX, currentY, &text[i], PLANE_BG);
            i += bytes_consumed;  // Avança os bytes consumidos
            currentX++;           // Move para a direita
        }
    }
}

// Carrega uma fonte alternativa para o jogo
void initFont(const TileSet *font_tileset, u8 palette_num) {
    // Carrega o tileset da fonte usando DMA
    VDP_loadTileSet(font_tileset, TILESET_OFFSET, DMA);

    // Define a paleta de texto
    VDP_setTextPalette(palette_num);  // Usa a paleta especificada pelo número
}
