#ifndef TRACK_H
#define TRACK_H

#include "constants.h"
#include "bluetooth.h"

/* PASSOS DO MAPEAMENTO:
  1) Trocar a booleana mapeamento em sensors.h para true e comentar a gambiarra pra problemas de mapemanto em pequenas curvas em mapeamento.h
  2) Colocar o robo para mapear com baixo PWM. Fazer isso pelo menos 2 vezes, ver se o resultado esta coerente e passar os resultados do apk pro Python
  3) Verificar se há algum resultado incoerente (tanto entre as tentativas quanto comparando com a pista) e copiar os valores do Python para as listas abaixo
  ATENCAO: O primeiro trecho da pista (logo apos a marcacao de inicio) tem mappingPath igual a 2. Os indices das listas a seguir estao acrescidos de 1
  4) Reescrever as gambiarra pra problemas de mapemanto em pequenas curvas caso necessario
  5) Trocar a booleana mapeamento em sensors.h para true, descomentar a gambiarra pra problemas de mapemanto em pequenas curvas em mapeamento.h E mudar criterios de parada na Main
  6) Fazer ajuste fino, antecipando os freios em retas onde a velocidade e maior, aumentando a velocidade em curvas abertas para ~1.5 e mudando a velocidade para acabar caso necessario
*/

float meusMultiplicadores[60] = {
  /* 2 */ PWM_MAX,/* 3 */ PWM_MAX,/* 4 */ 1.4,/* 5 */ 3.1,/* 6 */ PWM_MAX,
/* 7 */ 1.5,/* 8 */ 3,/* 9 */ 1.4,/* 10 */3.3 ,/* 11 */ 1.4,
/* 12 */ 2.6,/* 13 */ 3,/* 14 */ 1.3,/* 15 */ 1.5,/* 16 */ 3.3,
/* 17 */ 1.6,/* 18 */ 3.4,/* 19 */ 1.8,/* 20 */ 3.7,/* 21 */ 2,
/* 22 */ 1.6,/* 23 */ 2.8,/* 24 */ 1.3,/* 25 */ 1.4,/* 26 */ 1,
/* 27 */ 3,/* 28 */ 3.2,/* 29 */ 1.2,/* 30 */ PWM_MAX,/* 31 */ 1.5,
/* 32 */ 3.2,/* 33 */ 2.5,/* 34 */ 1.3,/* 35 */ 3.4,/* 36 */ 1.3,
/* 37 */ 2,/* 38 */ 1.2,/* 39 */ 3.6,/* 40 */ 3.6,/* 41 */ 3.6,/* 42 */ 0 
};

float velocidadeParaAcabar[60] = {
  /* 2 */ NAO_FREIA,/* 3 */ 1.3,/* 4 */ NAO_FREIA,/* 5 */ NAO_FREIA,/* 6 */ 0.4,
/* 7 */ NAO_FREIA,/* 8 */ 0.6,/* 9 */ NAO_FREIA,/* 10 */ 0.5,/* 11 */ NAO_FREIA,
/* 12 */ NAO_FREIA,/* 13 */ 0.1,/* 14 */ NAO_FREIA,/* 15 */ NAO_FREIA,/* 16 */ 0.6,
/* 17 */ NAO_FREIA,/* 18 */ 0.4,/* 19 */ NAO_FREIA,/* 20 */ 0.7,/* 21 */ NAO_FREIA,
/* 22 */ NAO_FREIA,/* 23 */ 0.5,/* 24 */ NAO_FREIA,/* 25 */ NAO_FREIA,/* 26 */ NAO_FREIA,
/* 27 */ NAO_FREIA,/* 28 */ 0.65,/* 29 */ NAO_FREIA,/* 30 */ 0.4,/* 31 */ NAO_FREIA,
/* 32 */ 1.3,/* 33 */ 0.4,/* 34 */ NAO_FREIA,/* 35 */ 0.5,/* 36 */ NAO_FREIA,
/* 37 */ NAO_FREIA,/* 38 */ NAO_FREIA,/* 39 */ NAO_FREIA,/* 40 */ NAO_FREIA,/* 41 */ NAO_FREIA,/* 42 */ NAO_FREIA
};

int comprimentoTotal[60] = {
  /*2(1,1)*/(1 - 10),/*3(87,81)*/(84 - 30),/*4(198,136)*/(167 - 10),/*5(160,155)*/(157 - 50),/*6(122,128)*/(125 - 63),
/*7(36,95)*/(65 - 10),/*8(209,221)*/(215 - 15),/*9(33,89)*/(61 - 10),/*10   (78,103)*/(90 - 10),/*11(34,80)*/(57 - 10),
/*12(259,272)*/(265 - 10),/*13(114,116)*/(115 - 60),/*14(39,155)*/(97 - 10),/*15(174,70)*/(122 - 10),/*16(339,320)*/(329 - 20),
/*17(147,44)*/(95 - 10),/*18(180,178)*/(179 - 10),/*19(40,165)*/(102 - 10),/*20(379,390)*/(384 - 10),/*21(73,46)*/(59 - 10),
/*22(141,70)*/(105 - 10),/*23(133,123)*/(128 - 10),/*24(115,49)*/(82 - 10),/*25(61,64)*/(62 - 10),/*26(42,99)*/(70 - 10),
/*27(149,155)*/(152 - 10),/*28(292,293)*/(292 - 70),/*29(43,109)*/(76 - 10),/*30(71,86)*/(78 - 5),/*31(44,103)*/(73 - 10),
/*32(291,298)*/(294 - 10),/*33(85,79)*/(82 - 30),/*34(117,51)*/(84 - 10),/*35(227,216)*/(221 - 0),/*36(108,41)*/(74 - 10),
/*37(37,27)*/(32 - 10),/*38(111,48)*/(79 - 10),/*39(276,270)*/(273 - 10),/*40(160,160)*/(160 - 10),/*41(10000,10000)*/(11 - 10),/*42(10000,10000)*/(1)
};

#endif
