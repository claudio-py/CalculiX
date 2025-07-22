/*******************************************************************************************
 *
 *   CalculiX v1.0.0 - a calculator aimed to calculate
 *
 *   LICENSE: Propietary License
 *
 *   Copyright (c) 2022 CalculiX technologies. All Rights Reserved.
 *
 *   Unauthorized copying of this file, via any medium is strictly prohibited
 *   This project is proprietary and confidential unless the owner allows
 *   usage in any other form by expresely written permission.
 *
 **********************************************************************************************/

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "external/calculator.h"
#include "external/converter.h"
#include "external/raygui.h"
#include <string.h>

//----------------------------------------------------------------------------------
// Declaracao antecipada das funcoes dos botoes
//----------------------------------------------------------------------------------
static void Button001();
static void Button002();
static void Button003();
static void Button004();
static void Button005();
static void Button006();
static void Button007();
static void Button008();
static void Button009();
static void Button010();
static void Button011();
static void Button012();
static void Button013();
static void Button014();
static void Button015();
static void Button016();
static void Button017();
static void Button018();
static void Button019();
static void Button020();
static void Button021();
static void Button022();
static void Button023();

// variavel de input/output
char boxText[128] = "";
//------------------------------------------------------------------------------------
// entrada principal do programa
//------------------------------------------------------------------------------------
int main() {
  // inicializacao
  //---------------------------------------------------------------------------------------
  int screenWidth = 360;
  int screenHeight = 500;
  // declara resolucao de tela
  InitWindow(screenWidth, screenHeight, "CalculiX");

  // CalculiX:controles de inicializacao
  //----------------------------------------------------------------------------------
  // variavel que controla modo de edicao da caixa de texto
  bool TextBox000EditMode = false;
  //----------------------------------------------------------------------------------
  // declara fps
  SetTargetFPS(60);
  // declara a escala de zoom
  GuiSetStyle(DEFAULT, TEXT_SIZE, 2 * GuiGetStyle(DEFAULT, TEXT_SIZE));
  //--------------------------------------------------------------------------------------

  //loop principal da janela 
  while (!WindowShouldClose()) //detecta botao esc ou o botao de fechar 
  {

    // inicia o desenho na tela
    //----------------------------------------------------------------------------------
    BeginDrawing();
//define cor de background
    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    // raygui:controle de desenho 
    //----------------------------------------------------------------------------------
    //define os tamanhos e posicoes das caixas
    if (GuiTextBox((Rectangle){0, 0, 360, 64}, boxText, 128,
                   TextBox000EditMode))
      TextBox000EditMode = !TextBox000EditMode;
    if (GuiButton((Rectangle){0, 72, 90, 72}, "1"))
      Button001();
    if (GuiButton((Rectangle){90, 72, 90, 72}, "2"))
      Button002();
    if (GuiButton((Rectangle){180, 72, 90, 72}, "3"))
      Button003();
    if (GuiButton((Rectangle){270, 72, 90, 72}, "."))
      Button004();
    if (GuiButton((Rectangle){0, 144, 90, 72}, "4"))
      Button005();
    if (GuiButton((Rectangle){90, 144, 90, 72}, "5"))
      Button006();
    if (GuiButton((Rectangle){180, 144, 90, 72}, "6"))
      Button007();
    if (GuiButton((Rectangle){270, 144, 90, 72}, "+"))
      Button008();
    if (GuiButton((Rectangle){0, 216, 90, 72}, "7"))
      Button009();
    if (GuiButton((Rectangle){90, 216, 90, 72}, "8"))
      Button010();
    if (GuiButton((Rectangle){180, 216, 90, 72}, "9"))
      Button011();
    if (GuiButton((Rectangle){270, 216, 90, 72}, "-"))
      Button012();
    if (GuiButton((Rectangle){0, 288, 90, 72}, "0"))
      Button013();
    if (GuiButton((Rectangle){90, 288, 90, 72}, "("))
      Button014();
    if (GuiButton((Rectangle){180, 288, 90, 72}, ")"))
      Button015();
    if (GuiButton((Rectangle){270, 288, 90, 72}, "x"))
      Button016();
    if (GuiButton((Rectangle){0, 360, 90, 72}, "Root"))
      Button017();
    if (GuiButton((Rectangle){90, 360, 90, 72}, "Pow"))
      Button018();
    if (GuiButton((Rectangle){180, 360, 90, 72}, "F->C"))
      Button019();
    if (GuiButton((Rectangle){270, 360, 90, 72}, "/"))
      Button020();
    if (GuiButton((Rectangle){0, 432, 120, 72}, "="))
      Button021();
    if (GuiButton((Rectangle){120, 432, 120, 72}, "Clear"))
      Button023();
    if (GuiButton((Rectangle){240, 432, 120, 72}, "< Back"))
      Button022();
    //----------------------------------------------------------------------------------
//finaliza o desenho
    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization / "des-inicializacao"
  //--------------------------------------------------------------------------------------
  CloseWindow(); //Fecha janela e contexto openGL 
  //--------------------------------------------------------------------------------------

  return 0;
}

//------------------------------------------------------------------------------------
// controle de definicao das funcoes (local)
//------------------------------------------------------------------------------------
static void Button001() { strcat(boxText, "1"); }
static void Button002() { strcat(boxText, "2"); }
static void Button003() { strcat(boxText, "3"); }
static void Button023() { strcpy(boxText, "\0"); }
static void Button005() { strcat(boxText, "4"); }
static void Button006() { strcat(boxText, "5"); }
static void Button007() { strcat(boxText, "6"); }
static void Button008() { strcat(boxText, "+"); }
static void Button009() { strcat(boxText, "7"); }
static void Button010() { strcat(boxText, "8"); }
static void Button011() { strcat(boxText, "9"); }
static void Button012() { strcat(boxText, "-"); }
static void Button013() { strcat(boxText, "0"); }
static void Button014() { strcat(boxText, "("); }
static void Button015() { strcat(boxText, ")"); }
static void Button016() { strcat(boxText, "*"); }
static void Button017() { strcat(boxText, "r("); }
static void Button018() { strcat(boxText, "^"); }
static void Button020() { strcat(boxText, "/"); }
static void Button004() { strcat(boxText, "."); }

// farenheight para celcius
static void Button019() { strcpy(boxText, fahrenheit_to_celsius(boxText)); }

// resultado fo calculo
static void Button021() { strcpy(boxText, backend(boxText)); }
// apaga o ultimo caractere da string
static void Button022() {
  size_t len = strlen(boxText);
  if (len > 0) {
    boxText[len - 1] = '\0';
  }
}
