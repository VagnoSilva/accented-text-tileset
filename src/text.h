#ifndef TEXT_H
#define TEXT_H

#include <genesis.h>

// Função para mapear o caractere para o índice da tile no tileset
size_t getCharacterTile(const char *text, int *bytes_consumed);

// Função para desenhar um caractere com seu tileset
void drawCharacter(int x, int y, const char *text, u16 plane);

// Função para desenhar texto com quebra de linha
void drawText(int x, int y, const char* text);

// Carrega uma fonte alternativa para o jogo
void initFont(const TileSet *font_tileset, u8 palette_num);

#endif // TEXT_H
