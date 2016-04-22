﻿/*----------------------------------------------------------------------------*
*	ÆÄÀÏ¸E:  sinMsg.cpp
*	ÇÏ´ÂÀÏ :  ´Ù¸¥¾ð¾ûÜÇ ÄÁ¹öÁ¯À» À§ÇØ ¸Þ¼¼Áö¸¦ °E?Ñ´?
*	ÀÛ¼ºÀÏ :  ÃÖÁ¾¾÷µ¥ÀÌÆ® 4¿E
*	Àû¼ºÀÚ :  ¹Ú»ó¿­
*-----------------------------------------------------------------------------*/

#include "sinLinkHeader.h"

sMESSAGEBOX	sMessageBox[100] = {
	{"Espaço insuficiente"},
	{"Excede o Peso limite"},
	{"Este item não pode ser usado"},
	{"Ouro insuficiente"},
	{"Este item não pode ser Mixado"},
	{"Excede o número Máx. de itens"},
	{"Troca cancelada"},
	{"Poções não podem ser armazenadas"},
	{"A Mixagem do Item falhou"},
	{"Obrigado pela sua contribuição"},
	{"Muitos Itens no Armazém"},
	{"Este item não pode ser usado no Aging"},
	{"A troca foi alterada, confira"},
	{"Excede o limite de Ouro"},
	{"O Aging falhou"},
	{"Status reiniciado"},
	{"A imagem do item não está pronta"},
	{"Você só pode reiniciar uma vez"},
	{"Somente para  membros de Clan"},
	{"Somente para  membros de Clan"},
	{"Nível insuficiente"},
	{"O Nível do Aging caiu -1"},
	{"O Nível do Aging caiu -2"},
	{"O Nível do Aging subiu +1"},
	{"O Nível do Aging subiu +2"},
	{"A Vel. de ATQ do Item foi reduzida"},
	{"   Mature o Item do Desafio    "},
	{"   HP +15"},
	{"   Quebra-cabeças completo    "},
	{"   Quebra-cabeças incorreto    "},
	{"   Nenhum item para vender  "},
	{"O item já foi vendido"},
	{"A loja pessoal do jogador já fechou"},
	{"   Vendedor não encontrado   "},
	{"    Número de itens incorreto     "},
	{"    Guerreiros não podem usar este item    "},
	{"    Guerreiras não podem usar este item    "},
	{" Guerreiras não podem apanhar este item "},
	{" Guerreiros não podem apanhar este item "},
	{"    O preço do item foi alterado  "},
	{"    Nenhuma recompensa para o Clan    "},
	{"  Você não está acima do Nível limite  "},
	{"     A Força Orb está sendo usada agora    "},
	{"     A função do Poder de Ataque está em teste    "},
	{"    A maturação da Força Orb está completa    "},
	{"    +5 Pontos de Status    "},
	{"    +1 Ponto de Habilidade    "},
	{" +1 Ponto de Habilidade, +5 Pontos de Status"},
	{" +2 Pontos de Habilidade, +5 Pontos de Status"},
	{"   Penalidade pela morte reduzida em 1%  "},
	{"   Vitalidade +40   " },
	{"    Você já está sob Desafio     " },
	{"Após o Nível 80, você ganhará 7 pontos por Nível." },
	{"Após o Nível 90, você ganhará 10 pontos por Nível." },
	{"Status reiniciado" },
	{"Missão incompleta pelo tempo" },
	{"Muitos soldados mercenários"},
	{"Está sendo usada nas Habilidades"},
	{"Transporte impossível. Por favor, chame um GM"},
	{"Este Item não pode ser vendido"},
	{"Não há taxas suficientes"},
	{"Você já respondeu a estas perguntas"},
	{"Obrigado por responder às perguntas"},
	{"O item é incompatível com seu Nível"},
	{"O item foi reconstruído"},
	{"Encontrado no confronto contra o Fúria"},
	{"Perdido no confronto contra o Fúria"},
	{"Itens de compensação"},
	{"Itens aliados não podem ser combinados"},
	{"Poderá ser usado futuramente"},
	{"Impossível cancelar enquanto sheltoms são usados"},
	{"Impossível carregar arma enquanto sheltoms são usados"},
	{"Tipo de poção repetido"},
};


sMESSAGEBOX2 sMessageBox2[20] = {
	{"Quantos itens","você deseja comprar?"},
	{"Quantos itens","você deseja vender?"},
	{"Quantos itens","você deseja abandonar?"},
	{"Quanto dinheiro","você deseja abandonar?"},
	{"",""},
	{"Quanto dinheiro"," você deseja trocar?"},
	{"Quanto dinheiro","você deseja transferir?"},
	{"Quanto dinheiro","você deseja retirar?"},
	{" Quantas estrelas ","você deseja comprar?"},
	{"  Quanto dinheiro  ","você deseja doar?"},
	{"Você deseja registrar","o preço de seu item? "},
	{"Quantos itens","você deseja comprar?"},  //°³ÀÎ»óÁ¡ Æ÷¼Ç
	{"Quanto dinheiro","você deseja retirar?"},
	{"  Você deseja ","comprar o item?"},
	{"  Você deseja ","comprar o item?"}, //14¹øÂ° ¿ä³Ñ¸¸ ÁøÂ¥
	{"Quanto dinheiro","você deseja investir?"},//°ø¼ºÀü µ·Ã£±â
	{"Quantos mercenários ","você deseja contratar?"} //¿ëº´ ±¸ÀÔ

};


sMESSAGEBOX3 sMessageBox3[MESSAGE3_MAX] = {
	{"Você deseja","adquirir as Habilidades?"},
	{"Deseja", "acabar o Desafio de classe?"}, //¿ä°Ç ÅÛ½ºÅ©·Ð
	{"Você aceita", "o Desafio?"},
	{"Você deseja","redistribuir?"},
	{"O evento"," custa"},
	{"Deseja","reiniciar seus STATUS?"},
	{"Você aceita", "o Desafio?"}, //¿ä°Ç ¸ð¶óÀÌ¿Â
	{"Você aceita", "o item?"}, //PostBox
	{"Nomear o", "Core de União?"}, //LinkCore
	{"Você aceita", "o item?"}, //RecvOthers
	{"Mover-se ao lugar", "indicado ?"}, //RecvOthers
	{"","Por favor, digite o número"}, //RecvOthers
	{"Deseja","comprar as Estrelas?"}, //RecvOthers
	{"Deseja", "vender o item?"}, //¾ÆÀÌÅÛ ÆÈ±â
	{"Você aceita", "o Desafio?"}, //¿ä°Ç 3Â÷Àü¾÷
	{" Deseja", "mesmo doar?"}, //¾ÆÀÌÅÛ ÆÈ±â
	{" Loja pessoal ", "fffffffffff"}, //¾ÆÀÌÅÛ ÆÈ±â
	{"Deseja", "trocar os itens? "}, //ÆÛÁñ·Î ¾ÆÀÌÅÛ±³È¯
	{"Deseja", "comprar o item?"}, //°³ÀÎ»óÁ¡ ¾ÆÀÌÅÛ»ç±â
	{"Deseja ", "cancelar o registro?"}, //°³ÀÎ»óÁ¡ ¾ÆÀÌÅÛ µî·Ï Ãë¼Ò
	{"Deseja", "trocar o item? "}, //¹Ùº§ÀÇ»Ô ¾ÆÀÌÅÛÀ¸·Î ±³È¯ÇÏ±â
	{"Deseja", "trocar o item? "}, //±¸¹ÌÈ£ ¾ÆÀÌÅÛÀ¸·Î ±³È¯ÇÏ±â
	{"Você aceita", "o Desafio?"}, //Level30
	{"Você aceita", "o Desafio?"}, //Level55
	{"Você aceita", "o Desafio?"}, //Level55_2
	{"Você aceita", "o Desafio?"}, //Level70
	{"Você aceita", "o Desafio?"}, //Level80
	{"Você aceita", "o Desafio?"}, //Level85
	{"Você aceita", "o Desafio?"}, //Level90
	{"Para onde", "você deseja ir?"}, //ÅÚ·¹Æ÷Æ®
	{"Você aceita", "o Desafio?"}, //Level80
	{"Você aceita", "o Desafio?"}, //Level90_2
	{"Você aceita", "o Desafio?"}, //4Â÷ÀüÁ÷
	{"Você aceita", "o Desafio de Classe?"}, //4Â÷ ÀüÁ÷
	{"Deseja", "trocar o item? "},//ÀÏ°ö°³ÀÇ Å©¸®½ºÅ»À» ¾ÆÀÌÅÛÀ¸·Î ±³È¯ÇÏ±â
	{"Deseja ser transportado", "a algum lugar?"},    //Teleport
	{"Gostaria de comprar","uma Torre de Cristais?"},      //Å©¸®½ºÅ» Å¸¿ö¸¦±¸ÀÔÇÏ´Ù.
	{"Você aceita", "o Desafio?"}, //A
	{"Você aceita", "o Desafio?"}, //B
	{"Você aceita", "o Desafio?"}, //C
	{"Você aceita", "o Desafio?"}, //D
	{"Você aceita", "o Desafio?"}, //E
	{"Você aceita", "o Desafio?"}, //F
	{"Você aceita", "o Desafio?"}, //G
	{"Deseja", "usar este item"},       //±¸¸Å ¾ÆÀÌÅÛ »ç¿ë
	{"Você aceita", "o Desafio?"},//100LVÄù½ºÆ® ¼öÇØ¿©ºÎ
	{"Você aceita", "receber o item?"},   //100LV¾ÆÀÌÅÛ
	{"Deseja","ir para o Campo de Batalha?"},   //100LVÄù½ºÆ® ´ë·ÃÀå ÀÌµ¿¿©ºÎ
	{"Deseja", "Teletransportar-se?"},
};


char *SkillMaster[4] = {
	"Aprender Habilidade",
	"Mudar de Classe",
	"Sair do jogo"

};
char *SkillMaster2[4] = {
	"",
	"",
	""

};

char *ReStartMsg[4] = {
	" Recomeçar no Campo de Batalha",
	" Recomeçar na Cidade",
	" Sair do jogo",
	" EXP. insuficiente"

};
char *ReStartMsg2[4] = {
	" Penalidade(Ouro/EXP.) 1%",
	"",
	"",
	" para recomeçar no Campo de Batalha",

};

char *SkillUseInfoText[10]= { "    Treinar Habilidade/Magia     "," Selecione um Habilidade     ","      Para treiná-la ou aprendê-la       ",
				"Pontos de Habilidade  ","   insuficientes " };
char *sinMsgSkillPoint = "    Pontos de Habilidade : %d";
char *sinMsgEliteSkillPoint = "Pontos Especiais : %d " ;
char *sinStateName[5]={"H P : %d/%d  ","M P : %d/%d  ","R E S : %d/%d  ","E X P : %d/%d  ","E X P : %dM/%dM  "};


char *TownName[3] = {"Voltar para Ricarten\r","Voltar paraNavisko\r","Voltar para Pillai\r"};

char *sinAbilityName[50] = {"Poder de Ataque:  \r","Vel. da Arma:\r","Alcance:  \r","Crítico:  \r","Taxa de Defesa:  \r","Taxa de Ataque:\r",
					"Absorção:  \r","Bloqueio:  \r","Velocidade:  \r","Integridade:\r","Regen de MP:  \r","Regen de HP :  \r",
					"Regen de RES:  \r","Res. Orgânica:  \r","Nature Type:  \r","Res. a Fogo:  \r","Res. a Gelo:  \r","Res. a Raio:  \r","Res. a Veneno: \r",
					"Water Type:  \r","Wind Type:  \r","HP Regen:  \r","MP Regen: \r","STM Regen: \r","Add. HP: \r",
					"MP Adicional:  \r","RES Adicional:  \r","Limite de Poções:  \r","Nível Necessário:  \r","Força Necessária:  \r","Inteligência Necessária:  \r",
					"Talento Necessário:  \r","Agilidade Necessária:  \r","Vitalidade Necessária:  \r"};


char *sinSpecialName[50] = {"Spec. ATK SPD:  \r","Crítico Adicional: \r","Def. Adicional:  \r","Abs Adicional:  \r",
						"Bloqueio Adicional  \r","Bônus Mágico:  \r","Vel. Adicional:  \r","R.Org. Adicional:  \r","Spec. Nature:  \r",
						"R. Fogo Adicional:  \r","R. Gelo Adicional:  \r","R. Raio Adicional:\r","R. Veneno Adicional:  \r","R. Água Adicional:  \r","R. Vento Adicional:  \r",
						"P. de ATQ Adicional:  \r","Taxa de ATQ ad.:  \r","Spec. RNG:\r","ATQ Org. Adicional:  \r","Spec. Nature ATK:  \r","ATQ Fogo Adicional\r","ATQ Gelo Adicional:  \r",
						"ATQ Raio Adicional:  \r","ATQ Veneno Adicional:  \r","ATQ Água Adicional:  \r","ATQ Vento Adicional:  \r","Bônus HP Max.:  \r","Bônus MP Max.:  \r",
						"Regen de HP:  \r","Regen de MP:  \r","Regen de RES:  \r","Preço de venda:  \r","Preço:  \r"};


char *SpecialName3 = "%s +. \r";
char *MixResultTitle = "Resultado da Mixagem";
char *NowLevelText = "Nível atual\r";
char *NextLevelText = "Próximo Nível\r";

char *NormalAttckName ="Ataque Padrão";
char *RequirLevel = "%s (Nv. Necess.:%d)\r";
char *UseItemGroupName = "Item compatível\r";

char *PoisonName = "Res. a Veneno:\r";
char *FireName = "Elemento Fogo:\r";
char *AttackAreaName = "Alcance:\r";
char *Attack_RateName = "Taxa de Ataque:\r";
char *Max_DamageIncre = "Bônus no dano Max.:\r";
char *DamageAddName = "Bônus no dano:\r";
char *Attack_SpeedAdd = "Bônus na Vel. de ATQ:\r";
char *IncreBlock = "Bônus no Bloqueio:\r";
char *CountinueTime = "Duração:\r";
char *UseManaName = "Custo de MP:\r";
char *DamageName = "Poder de Ataque:\r";
char *ShootingRangeName = "Alcance:\r";
char *IncreAsorb = "Bônus na Absorção:\r";
char *SecName = "seg.";
char *PlusDamageName = "Dano Adicional:\r";
char *HitNumName = "No. de Acertos:\r";
char *DecreLifeName = "HP diminui:\r";
char *Attck_RateAdd = "Poder de ATQ Ad.:\r";
char *Add_CriticalName = "Crítico Adicional:\r";
char *Push_AreaName = "Duração do recuo:\r";
char *FireAttackDamageAdd ="ATQ por Fogo:\r";
char *IceName = "Elemento Gelo:\r";
char *IceAttackDamageAdd = "ATQ por Gelo:\r";
char *StuneRate = "Probabilidade de Recuo:\r";
char *DefenseRateIncre = "Defesa aumenta:\r";
char *WeaponSizeName ="Tamanho da Arma:\r";
char *HwakRotationNum = "Número de voltas:\r";
char *WeaponSpeedAddName  = "Vel. de ATQ Ad:\r";
char *IncreAttack_RateName = "Poder de ATQ aumenta:\r";
char *ShootingNumName = "No. de disparos:\r";
char *SeriesShootingCount = "Fogo contínuo:\r";
char *UseStaminaName = "RES utilizada:\r";
char *MasterSkillMoneyName = "Preço da Habilidade:\r";

char *ItemAgingResult;		// << Add Point

char *sinGold ="%dOuro";
char *sinGiveItem5 = "Recompensa\r";
char *sinCopyItem5 = "Item duplicado\r";
char *sinNum7 = "RND";
char *sinSkillPointName = "Ponto de Habilidade";	// << Add Point
char *sinG_Pike_Time3 = "Tempo de Efeito:\r";	// << Add Point
char *sinItemLimitTimeOverMsg = "O tempo limite acabou\r";
char *sinDeadSongPyeunEat = "Mas, que meleca! Isso não presta!";
char *SmeltingResultTitle = "Resultado da Fundição";		// pluto Á¦·Ã
char *ManufactureResultTitle = "Resultado da Fabricação";	// pluto Á¦ÀÛ
char *ManufacturingTitle = "Aleatório";	//fuck pluto

/////////////////// ¸ð¶óÀÌ¿Â ½ºÅ³ TextÃß°¡
char *sinMaxDamageAdd = "Dano Máx. adicional\r";
char *sinDamagePiercing = "(ATQ Piercing)\r";
char *sinAddStamina = "RES adicional:\r";
char *sinAddLife = "Regen de HP:\r";
//char *sinPartyHealDoc = " Em grupos, a exp. será recuperada:\ tanto quanto 1/3 do HP recuperado:\r";
char *sinDamageUndead50 = "50% a mais de dano contra mortos-vivos:\r";
char *sinNumCount2 = "número:\r";
char *sinNumCount3 = "qtde.\r";
char *sinDecreDamage = "Prejuízo ao dano:\r";
char *sinConvert4 = "Taxa de conversão:\r";
char *sinIncreElement = "Adição a res. elemental:\r";
char *sinPartyIncreElement = "Membros do grupo ganham 50% de res. elemental\r";
char *sinFireBallDamage2 = "(100% de dano ao inimigo atacado)\r";
char *sinFireBallDamage3 = "(área de alcance 60 (1/3 dano))\r";
char *sinAddDamage7 = "dano adicional:\r";
char *PlusDamageWeapon = "arma";
char *SparkDamage = "dano por unidade:\r";
char *SparkNum7 = "# máx. de fagulhas:\r";
char *StunRage = "Alcance do recuo:\r";
char *RecvPotion7 = "Poção";

//¸ð¶óÀÌ¿Â 2Â÷½ºÅ³
char *PartyArea7 ="Alcance no grupo:\r";
char *Area17 ="Área:\r";
char *AddAttack_Rate9 ="Taxa de ATQ:\r";
char *PiercingRange ="Alcance do Piercing:\r";
char *IncreDefense = "Aumento na Def.:\r";
char *AddShootingRange = "Aumento no alcance:\r";
char *LifeAbsorb = "Absorção do HP.:\r";
char *FireDamage = "Dano por Fogo:\r";
char *IceDamage = "Dano por Gelo:\r";
char *LightningDamage = "Dano por Raio:\r";
char *AddManaRegen = "Regen de Mana ad.:\r";
char *LightNum = "# de Raios:\r";
char *ReturnDamage = "Dano refletido:\r";
char *Area18 = "Alcance:\r";
char *IncreMana5 = "Aumento na Mana:\r";
char *Area19 = "Alcance:\r";
char *HelpTitle8 = " A J U D A ";
char *QuestTitle8 = " D E S A F I O ";
char *ResearchTitle8 = "B U S C A";
char *TeleportTitle8 = "TELEPORT";

char *IncreWeaponAttack_RateName = "Aumento na Taxa de ATQ:\r";
char *AddMaxDamage = "Aumento no dano Máx.:\r";
char *LinkCoreName = "Transportar-se ao membro do Clan\r";
char *DesLinkCore = "Nome:";
char *Itemul = "item";
char *ItemRecvOk = "Você aceita isto?";
char *Money5 = "ouro";
char *Exp5 = "EXP.";
char *WingItemName[] = {"Asa de Metal","Asa de Prata","Asa de Ouro","Asa de Diamante", "Asa do Caos"};
char *WarpGateName[] = {"Portal","Ricarten","Pillai","Terra do Crepúsculo","Floresta Bamboo","Vila Ruinen","Cidade de Navisko","Terra Proibida", "Vila Eura"};
char *sinWarningName5 = "A  V  I  S  O";
char *ClanCristalName = "Somente para membros de Clan\r";
char *NowMyShopSell = "A venda\r";
char *SecretNumName = "Número de autenticação";
char *MyShopExpDoc7 = "Detalhes da Loja";
char *NotAgingItem2 = "'Impossível aplicar o Aging";
char *ExpPercent2 = "EXP : %d.%d%s";
char *ExpPercent3 = "EXP : %d.0%d%s";
char *Mutant7   =   "Mutante";
char *Mechanic7 = "Mecânico";
char *Demon7    = "Demônio";
char *Nomal7    = "Normal";
char *Undead7    = "Morto Vivo";
char *MonsterAddDamage2 = "dano adicional)\r";
char *MonsterAddDamageFire = "dano por fogo adicional)\r";
char *HaWarpGateName[] = {"Castelo Bless",};
char *FallGameName = "Twisted forest";
char *EndlessGameName = "Torre sem Fim";
char *LookHelp = ">>more...";

//º°¾ÆÀÌÅÛ
char *HoldStarNumDoc = "O número das suas estrelas";
char *StarItemBuy7   = "Comprar as estrelas";
char *ChangeMoney7   = "(Preço de uma estrela : 100000)";



//3Â÷ ½ºÅ³
char *LightningDamage2 = "Dano por Raio:\r";
char *SheildDefense = "Aumento na Def.:\r";

char *DemonDamage4  = "ATQ Ad. em Demônios:\r"; 
char *PoisonDamage3  = "Dano por Veneno ad.:\r";
char *PikeNum4       = "Número de Foices:\r";
char *poisoningTime  = "Duração do veneno:\r";
char *PlusCriticalName = "Crítico adicional:\r";
char *SpiritFalconDamage2 = "Dano adicional:\r";
char *LifeGegenPlus = "Aumento no HP:\r";
char *PlusDamage4 = "Aumento no Dano:\r";
char *PiercingPercent = "Taxa de perfuração:\r";
char *DamageToLife = "Dano convertido:\r";
char *MyMonster7 = "Dominação:\r";
char *LifeIncre4 = "Aumento no HP:\r";
char *UndeadDamage3 = "Dano ad. em Mortos vivos:\r";

char *AttackNum3     = "No. de ATQs:\r";
char *AttackRateMinus = "Deficit de Taxa de ATQ:\r";
char *MaxDamagePlus2 = "MaxDamagePlus:\r";
char *LightningAddDamage3 = "Dano ad. por Raio:\r";
char *ReLifePercent4    = "Taxa de Rec. de HP:\r";
char *GetExp3    = "Exp. adquirida:\r";
char *ResurrectionChar4    = "Taxa de restauração";
char *ExtinctionPercent2    = "% de extinção:\r";
char *ExtinctionAmount2    = "Taxa de extinção:\r";
char *IncreLifePercent2   = "% ad. de HP:\r";

char *ReduceDamage3 = "Redução no dano:\r";
char *IncreMagicDamage = "Aumento no dano Mágico:\r";
char *AttackDelay3   = "Atraso no ATQ:\r";

char *FireDamage2 = "Dano por Fogo:\r";
char *IceDamage2 = "Dano por Gelo:\r";
char *AddDefense8 = "Def. Adicional:\r";
char *SkillDamage5 = "Dano da Habilidade:\r";

char *FireDamage3 = "Dano por Fogo:\r";
char *AddSpeed7 = "Vel. adicional:\r";
char *AfterDamage7 = "pós dano:\r";
char *MonsterSight7 = "percepção inimiga:\r";

//4Â÷½ºÅ³
//ÅÛ½ºÅ©·Ð
char *LinghtingAddDamage4="Dano ad. por Raio:\r";
char *MaxBoltNum4="Voltagem Max.:\r";
char *AddAbsorb4="Abs. adicional:\r";
char *IncreArea4="Alcance adicional:\r";
char *IncreAttack4="Poder de ATQ Ad.:\r";
char *GolemLife4="HP+Força:\r";


char *SubAbsorb4="Redução da Abs.:\r";
char *AreaDamage4="Alcance do ATQ:\r";
char *MaxAttackNum4="Acertos max.:\r";
char *LifeUp4="Vitalidade adicional:\r";

char *CriticalAddDamage4="Crítico adicional:\r";
char *MonstervsSubCritical4="Redução do ATQ:\r";
char *ChargingAddPercentDamage4="Taxa de ATQ ad. por Carga:\r";

char *IncreEvasionPercent4="Esquiva adicional:\r";
char *AddShadowNum4="Furtividade ad.:\r";

char *WolverinLife4="Vitalidade:\r";
char *WolverinRate4="Precisão:\r";
char *WolverinDefense4="Defesa:\r";
char *AddEvasion4="Esquiva adicional:\r";
char *AddDamage4="Taxa de ATQ ad.:\r";
char *FalconAddDamage4="Taxa de ATQ ad. do Falcão:\r";

//¸ð¶óÀÌ¿Â
char *IncreAttackAbsorb4="Abs adicional:\r";

char *RectAngleArea4="Desfere ataques precisos e fatais:\r";

char *MonsterSubSpeed4="Redução da vel. do ATQ:\r";


char *AddLifeRegen4="Regen de HP ad.:\r";
char *AddManaRegen4="Regen de MP ad.:\r";
char *MagicArea4="Alcance mágico:\r";
char *ChainNum4="No. de inimigos afetados:\r";
char *ChainRange4="Efeito nos inimigos feridos:\r";
char *UndeadDamageAbsorb4="Abs ad. contra mortos vivos:\r";
char *UserBlockPercent4="Bloqueio adicional:\r";

char *SecondIncreMana4="Regen de Mana por seg.:\r";
char *FireDamage4="Taxa de ATQ por Fogo:\r";
char *DecreSpeed4="Redução na Vel.:\r";
char *DecreAttack4="Redução na Taxa de ATQ:\r";
char *AddPercentDamage4="Taxa de ATQ ad.:\r";


//½ºÅ³ ÀÎ°è¿ë
char *ChainDamage3    = "Dano por faíscas)\r";
char *BrandishDamage3 = "Dano pelo Brandish";
char *PhysicalAbsorb3 = "Absorção física";
char *SparkDamage3    = "Dano pelas fagulhas";
char *BrutalSwingCritical3   = "Crítico brutal";
char *Attck_RateAdd5 = "Attck_RateAdd:\r";
char *Triumph4="Triunfo";
char *Use4="Você ";
char *DivineShield4="Bloqueio do escudo divino";

//Äù½ºÆ® ¸ó½ºÅÍ
char *QuestMonsterName[] = {"Bargon","Guerreiro do Mal","Decapitador","Besouro Blindado ","Esqueleto Patrulheiro","Titan",
                         "Monstros da Terra Esquecida","Monstros de Oásis","Monstros do 1° Andar do Calabouço Antigo"};


//¿ä°Ç ¹ìÇÁ¿ë +_+
char *VampOption[10] ={"Increaseagaing%: \r","Vampiro: \r","Danorelativo:\r","ReduçãodeHP: \r","Sorte: \r"};


char *Quest3ItemName[]={"Caça demônios","Vingador antigo","Mito","Lado demoníaco","Anaconda","Espada de Platina","Fantasma"};

char *Quset3ItemDoc15 = "Os itens do Desafio";
char *Quset3ItemDoc16 = " se foram ";

char *SparkDamage10 = "Dano das faíscas:\r";

char *RequirLevel3 = "(Nv.Necessário:%d)\r";

char *RecvItemTT = "Recebo o Item";
char *PuzzleEvent5 = "Cartão completo";
char *BabelHorn = "Chifre de Babel";
char *NineTailFoxItem = "Amuleto das Nove Caudas";
char *ChristMas = "Chiny Powder";		//Å©¸®½º¸¶½ºÀÌº¥Æ®
char *MyShopItemSell5 = "O Preço: \r";
char *CristalItem     ="os 7 Cristais";//ÀÏ°ö°¡Áö Å©¸®½ºÅ» º¸Àº


char *sinDeadCandyEat = "Que meleca! O gosto é horrível!";

char *PotionCntDoc2 = "Poções";
char *ItemPrice7 = "Preço do Item";

char *BuyMyShopItem7 = "%s comprou %d do vendedor %s por %d";
char *BuyMyShopItem8 = "%s comprou %d do vendedor %s por %d";
char *ItemName87 = "item";

char *sinEvent87 = "Desafio do Colecionador";
char *sinAddDamageItem = "P. de ATQ ad.: \r";
char *sinContinueTimeItem = "Duração: \r";
char *BuyPotionMoney = "Ouro:";
char *BuyPotionKind = "Número:";
char *SheltomName2[] = {"Lucidy","Sereno","Fadeo","Sparky","Raident","Transparo","Murky","Devine","Celesto", "Mirage", "Inferna", "Enigma"};

char *AddPercentDamage3= "P. de ATQ ad.%: \r";

char *sinLuckyBox = "Saco da Sorte";
char *SodMessage_Etc[]= {
			"A tarifa do torneio de Bellatra irá para",
			"Somente os Líderes de Clan recebem",
			"Agora, a taxa é %d%s.",
			};

char *SodMessage_Clan[] = {
			"O lucro total é de ",
			"Bellatra distribuiu a tarifa ontem.",
			};

char *SodMessage_Master[] = {
			"O total do prêmio é de",
			"Bellatra distribuiu a tarifa ontem.",
			"Deseja retirar?",
			};

char *OtherClanMaster[] = {
			"O Clan ainda possui a Tarifa",
			"ajuste o desconto.",
			};
/*----------------------------------------------------------------------------*
*			Å×½ºÆ®			°ø¼ºÀü ¸Þ´º¸Þ¼¼Áö
*-----------------------------------------------------------------------------*/
char *SiegeMessage_Taxrates[] = {
	"A tarifa foi mudada para %d%s.",
	"O total das tarifas acumulou",
	"Ouro.",
	"Gostaria de arrecadar a tarifa?",
	"A tarifa agora é de %d%s.",
};
char *SiegeMessage_MercenrayA[] = {
	"Milícia de Ricarten",
	"Esta é a Milícia",
	"São fracos, porém, eficientes.",
	"Qtde. máx. de Mercenários: 20 / %d",
};
char *SiegeMessage_MercenrayB[] = {
	"Guarda de Ricarten",
	"O preço e as habilidades são equilibrados",
	"São soldados melhor treinados para combate.",
	"Qtde. máx. de Mercenários: 20 / %d",
};
char *SiegeMessage_MercenrayC[] = {
	"Membro da Guarda Imperial",
	"Mercenários especializados em grandes Guerras.",
	"São os melhores soldados que você encontrará.",
	"Qtde. máx. de Mercenários:20 / %d",
};
char *SiegeMessage_TowerIce[] = {
	"Torre de Cristais de Gelo",
	"O Gelo reduz a movimentação",
	"mas aumenta a velocidade de ATQ.",
	"Posicionamento dos Cristais de Gelo completo",
};
char *SiegeMessage_TowerFire[] = {
	"Torre de Cristais de Fogo",
	"O Fogo causa danos terríveis",
	"mas não possui nenhum efeito extra.",
	"Posicionamento dos Cristais de Fogo completo",
};
char *SiegeMessage_TowerLighting[] = {
	"Torre de Cristais de Raio",
	"O Raio diminui a velocidade de ATQ e",
	"e de movimentação dos adversários.",
	"Posicionamento dos Cristais de Raio completo",
};
char *SiegeMessage_MerMoney   = "Custo por Mercenário: ";
char *SiegeMessage_TowerMoney = "Custo por Torre: ";

char *sinClanMaster7 = "Líder do Clan";
char *sinPrize7 = "Recompensa:  ";

char *sinLevelQuestMonster[]={"Bargon","Muffin","Abelha Selvagem","Typhoon","Ratoo","Grotesco","Punhos de Aço"};
char *sinLevelQusetDoc="Quest> %s %d monstros adicionados";

char *sinTeleportDoc[] = {"Entrada da Caverna dos Cogumelos","Entrada da Caverna das Abelhas","Terra Maldita","Terra Proibida"};
char *sinLevelQuest90_2Mon[] = {"Omega","Máquina-D","Montanha"};
char *sinLevelQusetDoc2="<Quest> Cacem %d dos %s";
char *sinLevelQusetDoc3="<Quest> Apanhem os itens dos %s";
char *sinLimitTiem2 = "Tempo: \r";
char *sinMinute2 = "minutos";
char *sinDay4    = "dia";
char *sinHour4   = "hora";
char *sinLevelQusetDoc4="<Quest> Conclua a caça de %d dos %s";

sinSTRING sinTestMsg7("Ha ha ha, seus otários... vocês não valem nada!");

char *sinMonCodeName[]={
	0,
	"Bargon",
	"Esqueleto Guerreiro",
	"Decapitador",
	"Besouro Blindado",
	"Esqueleto Patrulheiro",     //5
	"Titan",
	"Muffin",
	"Abelha Selvagem",
	"Typhoon",
	"Ratoo",                //10
	"Grotesco",
	"Punhos de Aço",
	"Omega",
	"Maquina-D",
	"Montanha",              //15
	"Cavaleiro do Mal",
	"Caramujo Rijo",
	"Caramujo Espinhoso",
	"Múmia",
	"Guardião Maldito",             //20
	"Figon",
	"Gigante de Pedra",
	"Golem de Pedra",
	"Guardião de Aço",
	"Centauro Arqueira",       //25
	"Centauro-L", 
	"Abelha do Mal",
	"Cavaleiro Fantasma",
	"Pesadelo",
	"Bruxa",                //30
	"Duende Guerreiro",
	"Dawlin",
	"Stygian",
	"Incubus",
	"Golem de Aço",           //35
	"Espectro Negro",
	"Metron",
	"Lorde Minotauro",
	"Dusk",
	"Ogro Mecânico",            //40
	0,
};

char *ChargingTime4="Vel. ad. por carga:\r";
char *AddIceDamage4="Dano ad. por Gelo:\r";
char *IceTime4="Tempo de recuo:\r";
char *sinAttackType = "Tipo de ATQ:\r";
char *sinAttackType2[2] ={"Manual\r","Automático\r"};

char *Won="Ouro";

//°ø¼ºÀü ¾ÆÀÌÅÛ ¼³¸í
char *CastlItemInfo[] ={
	{"Invencibilidade temporária\r"},
	{"Porcentagem crítica\r"},
	{"Esquiva adicional\r"},
	{"Restaura RES,HP,MP\r"},
	{"Ressurge imediatamente.\r"},
	{"ATQ da Torre de Cristais\r"},     //
	{"Bônus de 20% no ATQ\r"},
	0,
};
char *CastlItemInfo2[] ={
	{"reduz o ATQ pela metade\r"},
	{"aumento de 5%.\r"},
	{"aumento de 5%.\r"},
	{"dos membros do grupo.\r"},
	{"\r"},
	{"reduzido em 50%.\r"},
	{"contra o alvo.\r"},
	0,
};


char *CharClassTarget[8]={
	"Alvo(Lutador)\r",
	"Alvo(Mecânico)\r",
	"Alvo(Pike)\r",
	"Alvo(Arqueira)\r",
	"Alvo(Cavaleiro)\r",
	"Alvo(Atalanta)\r",
	"Alvo(Mago)\r",
	"Alvo(Sacerdotisa)\r",
};
char *AttributeTower[4]={
	"Attribute(Fogo)\r",
	"Attribute(Gelo)\r",
	"Attribute(Raio)\r",
	"Duração:%dSeg\r",
};

char *SiegeMessage_MerComplete ="Posicionamento dos Mercenários Completo";

//ÃÊº¸ Äù½ºÆ®
char *haQuestMonsterName[]={
	"Hopi",
	"Coelho",
	"Duende Macabro",
	"Duende Selvagem",
	"Esqueleto",
	"Esqueleto do Mal",
	"Ciclope",
	"Bargon",
	0,
};
char *ha100LVQuestItemName[]={
	"Machado Minotauro",
	"Garra Extrema",
	"Martelo Dragão",
	"Foice Infernal",
	"Arco da Vingança",
	"Espada Imortal",
	"Lança Salamandra",
	"Cajado Gótico",
	0,
};
//100LVÄù½ºÆ® (Ç»¸®ÀÇ È¯¿µ)
char *ha100LVQuestMonterName[]={
	"Monstros de Perum",
	"Monstros de Gallubia", 
	0,
};
char *ha100LVQuestName = "Fantasma do Fúria";

//ÇÁ¸®¹Ì¾ö À¯·áÈ­ ¾ÆÀÌÅÛ
char *PremiumItemDoc[][2]={
	{"Redistribui\r"     ,"pontos de Status.\r"},     //½ºÅÈ ÃÊ±âÈ­ ¾ÆÀÌÅÛ
	{"Redistribui\r"     ,"pontos de Habilidade.\r"},    //½ºÅ³ ÃÊ±âÈ­ ¾ÆÀÌÅÛ
	{"Redistribui\r"," pontos de St e Hab.\r"},     //½ºÅÈ/½ºÅ³ ÃÊ±âÈ­ ¾ÆÀÌÅÛ
	{"Ressuscita \r"    ,"personagens mortos.\r"},        //ºÎÈ° ¾ÆÀÌÅÛ
	{"Invencibilidade temporária\r","reduz o ATQ em 50%\r"},
//ÀÌÅÍ³Î ¶óÀÌÇÁ
	{"Aumento de 5% \r"    ,"no crítico.\r"},		//ÆäÀÌÆ² ¿¡Áö
	{"Aumento de 5% \r"         ,"na esquiva.\r"},		//¿¡¹öÆ® ½ºÅ©·Ñ
};

char *UpKeepItemDoc[] = {
	"Tempo restante : %dMin.",
	"Tx. de Drop %d%s adicional",
	"Exp. %d%s adicional",
	"Ataque %d%s adicional",
	"Converte % do dano em HP",
	"Converte % do dano em MP",
	"Aumenta a capacidade em d%",
	"Reduz o uso de MP em %d%s",
	0,
};
char *UpKeepItemName[] = {
	"Olho Mágico",
	"Poção da Experiência",
	"Presa de Vampiro",
	"Drena Almas.",
	"Poder de Awell",
	"Poção de redução de Mana",
	"Fênix",
	0,
};

//¹ÚÀç¿E- ¼ö¹Ú ¸ð¾Æ¿À±E
char *WatermelonItem = "7ŒÂ‚ÌƒXƒCƒJ‚ð"; // ¼ö¹Ú 7°³
										//¹ÚÀç¿E- È£¹Ú ¸ð¾Æ¿À±E
char *PumpkinItem = "7ŒÂ‚Ì‚©‚Ú‚¿‚á‚ð"; // È£¹Ú 7°³
									   //¹ÚÀç¿E- ¹ãÇÏ´ÃÀÇ ¼Ò¿EÀÌº¥Æ®
char *StarItem = "7ŒÂ‚Ì¯‚Ì‚©‚¯‚ç"; // º° 7°³
									//Àåº° - ¹ß·»Å¸ÀÎ ÃÊÄÝ¸´ ¸ð¾Æ¿À
char *ValentineItem = "7ŒÂ‚Ìƒ`ƒ‡ƒRƒŒ[ƒg "; // ¹ß·»Å¸ÀÎ ÃÊÄÝ¸´ 7°³
											//¹ÚÀç¿E- ±â°£¸¸·E
char *PristonAlphabetItem = "7ŒÂ‚Ì ƒAƒ‹ƒtƒ@ƒxƒbƒg‚ð";
char *CandydaysItem = "7ŒÂ‚ÌƒLƒƒƒ“ƒfƒB‚ð";
char *ExpireItem = "ŠúŠÔ–ž—¹";

char *MagicalGreenEmeraldItem = "7ŒÂ‚ÌƒGƒƒ‰ƒ‹ƒh‚ð";
char *MagicalGreenJadeItem = "7ŒÂ‚ÌƒqƒXƒC‚ð";

char *TearOfKaraItem = "7ŒÂ‚ÌƒJƒ‰‚Ì—Ü‚ð"; //- Here (^_^)

char *FindinvestigatorItem = " ";  //- Here (^_^)
char *FindinvestigatorNineItem = "ƒiƒCƒ“ƒyƒ“ƒ_ƒ“ƒg";  //- Here (^_^)
char *FindinvestigatorTaleItem = "ƒeƒCƒ‹ƒyƒ“ƒ_ƒ“ƒg";  //- Here (^_^)

char *BillingMagicForceName = "ƒ}ƒWƒbƒNƒtƒH[ƒX";
char *MagicForceName = "ƒ}ƒWƒbƒN";

char *InventoryFull = "O inventário está cheio.";
char *InvenSpaceSecure = "‹óŠÔ‚ðŠm•Û‚µ‚Ä‚­‚¾‚³‚¢";

// ¹ÚÀç¿E- ºÎ½ºÅÍ ¾ÆÀÌÅÛ 
// ¾ÆÀÌÅÛ Á¤º¸ ¸Þ½ÃÁE
char *BoosterItemInfo[] = {
	{ "ˆê’èŽžŠÔA¶–½—Í‚ð\r" },
	{ "ˆê’èŽžŠÔA‹C—Í‚ð\r" },
	{ "ˆê’èŽžŠÔA‘Ì—Í‚ð\r" },
	0,
};
char *BoosterItemInfo2[] = {
	{ "15%ã¸‚³‚¹‚é\r" },
	{ "15%ã¸‚³‚¹‚é\r" },
	{ "15%ã¸‚³‚¹‚é\r" },
	0,
};