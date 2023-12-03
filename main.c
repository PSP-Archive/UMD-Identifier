/* UMD Identifier v0.1 by harleyg - http://www.linuxlabs.co.uk */

#include <stdio.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspumd.h>
#include <pspgu.h>
#include <png.h>
#include "graphics.h"

PSP_MODULE_INFO("UMD Identifier v0.1", 0, 1, 1);

#define printf pspDebugScreenPrintf
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define RGB(r, g, b) ((r)|((g)<<8)|((b)<<16))

/* Exit callback */
int exit_callback(int arg1, int arg2, void *common) {
	sceKernelExitGame();
	return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp) {
	int cbid;
	cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void) {
	int thid = 0;
	thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0) {
		sceKernelStartThread(thid, 0, 0);
	}
	return thid;
}

int main(void) {
	int i;
	int j;
	int x = 0;
	int y = 0;
	Color red = RGB(255,0,0);
	Color blue = RGB(0,0,255);
	Color green = RGB(0,255,0);
	Color black = RGB(0, 0, 0);
	char buffer[200];
	pspDebugScreenInit();
	SetupCallbacks();
	printf("Initializing...");
	j = sceUmdCheckMedium(0);
	if(j == 0)
	{
		sceUmdWaitDriveStat(UMD_WAITFORDISC);
		printf("Please insert UMD!");
	}
	sceUmdActivate(1, "disc0:");
	sceUmdWaitDriveStat(UMD_WAITFORINIT);
	char k[49];
	char *codes[] = { 
		"UCAS-40021", "ULAS-42043", "ULES-00288", "UCAS-40004",
		"ULUS-10034", "UCAS-40036", "ULES-00219", "ULJS-00030", 
		"UCAS-40011", "UCJS-18005", "UCAS-40024", "UCAS-40049", 
		"UCAS-40038", "UCAS-40066", "ULAS-42015", "ULAS-42019", 
		"UCAS-40046", "ULAS-42053", "ULJM-08006", "ULKS-46016", 
		"ULJS-00027", "ULES-00168", "ULES-00169", "ULES-00195", 
		"UCAS-40054", "TCPS-10141", "UCAS-40073", "TCPS-10157", 
		"UCAS-40030", "TCPS-10121", "UCJS-18004", "UCJS-18002", 
		"ULKS-46085", "ULES-00119", "ULES-00118", "UCAS-40043", 
		"TCPS-10129", "ULKS-46054", "ULES-00161", "ULAS-42004", 
		"ULKS-46011", "ULAS-42023", "ULAS-42045", "ULAS-42044", 
		"UCAS-40026", "ULJM-05111", "UCAS-40060", "ULAS-42034",
		"ULAS-42041", "ULAS-42040", "ULKS-46064", "ULES-00182", 
		"ULKS-46040", "ULAS-42025", "ULAS-42027", "ULJM-05117", 
		"TCPS-10156", "UCAS-40016", "UCAS-40065", "UCAS-10054", 
		"ULAS-42042", "UCAS-40079", "UCAS-40044", "TCPS-10138", 
		"TCPS-10143", "UCAS-40042", "UCJS-10036", "ULKS-46001", 
		"ULES-00143", "ULJS-19003", "UCJS-10018", "ULJM-05052", 
		"UCAS-40053", "ULES-00199", "UCAS-40076", "ULKS-46005", 
		"ULJS-19005", "ULAS-42006", "ULJM-18005", "ULJM-05119", 
		"ULAS-42021", "ULKS-46036", "ULAS-42052", "ULAS-42007", 
		"ULJM-08001", "ULAS-42035", "UCAS-40002", "UCKS-45004", 
		"UCJS-18001", "UCKS-45010", "UCAS-40056", "UCAS-40010", 
		"ULAS-42033", "UCAS-40062", "ULAS-42001", "ULKS-46006", 
		"UCAS-40057", "UCAS-40061", "ULAS-42022", "ULES-00157", 
		"ULAS-42003", "ULKS-46007", "ULAS-42031", "ULAS-42005", 
		"ULJM-05073", "ULES-00036", "TCPS-10145", "ULUS-10118", 
		"ULKS-46062", "ULES-00213", "UCAS-40041", "UCAS-40052", 
		"UCAS-40007", "UCJS-18007", "UCAS-40009", "UCJS-18003", 
		"ULAS-42038", "UCAS-40031", "UCAS-40068", "UCES-00019", 
		"ULAS-42024", "UCAS-40006", "TCPS-10102", "TCPS-10144", 
		"ULJM-05090", "ULKS-46002", "ULJM-08004", "ULAS-42056", 
		"UCAS-40015", "UCKS-45002", "UCAS-40001", "ULAS-40001", 
		"ULJS-19002", "ULAS-42017", "ULAS-42050", "ULAS-42036", 
		"ULAS-42049", "UCAS-40003", "UCJS-18006", "UCAS-40019", 
		"UCAS-40047", "ULJS-00061", "ULJM-08008", "ULAS-42048", 
		"UCAS-40027", "UCAS-40014", "ULJM-05131", "ULAS-42010", 
		"ULJM-08007", "UCAS-40050", "ULJS-00053", "UCAS-40058", 
		"UCAS-40075", "UCAS-40017", "UCAS-40029", "UCAS-40013", 
		"TCPS-10105", "ULKS-46020", "ULES-00022", "ULJS-00023", 
		"ULAS-42026", "ULAS-42037", "ULAS-42039", "UCAS-40022", 
		"UCAS-40040", "ULJM-05076", "TCPS-10137", "ULKS-46056", 
		"UCAS-40005", "ULJS-19004", "UCAS-10009", "UCAS-40039", 
		"UCAS-40045", "UCAS-40020", "ULJM-05122", "ULAS-42028", 
		"ULES-00198", "ULAS-42029", "ULAS-42002", "ULAS-42020", 
		"ULAS-42046", "ULES-00033", "ULES-00034", "ULAS-42012", 
		"UCAS-40072", "TCPS-10155", "ULKS-46015", "ULES-00042", 
		"UCAS-40069", "ULJM-08002", "ULAS-42051", "ULKS-46033", 
		"UCAS-40012", "UCKS-45008", "ULAS-42018", "ULKS-46024", 
		"ULAS-42057", "ULJM-08009", "UCAS-40070", "ULES-00189", 
		"UCAS-40067", "UCAS-40055", "UCAS-40034", "ULAS-42011", 
		"TCPS-10139", "ULUS-10067", "ULUS-10025", "ULUS-10053",
		"UCUS-98618", "UCUS-96601", "ULES-00176", "ULUS-10022",
		"ULUS-10020", "ULUS-10054", "UCUS-98615", "ULUS-10041",
		"UCUS-96823", "ULUS-10025", "ULUS-10004", "ULUS-10007",
		"ULUS-10019", "ULUS-10003", "UCUS-98612", "ULUS-10016",
		"UCUS-98614", "ULUS-10001", "ULUS-10051", "ULUS-10086",
		"ULUS-10077", "ULUS-10070", "ULUS-10006", "ULES-00108",
		"ULES-00151", "UCES-00001", "ULUS-10061", "ULUS-10014",
		"ULUS-10063", "ULES-00146", "ULUS-10042", "ULES-00037",
		"ULES-00193", "ULES-00175", "ULUS-10044", "ULJM-05062",
		"ULUS-10092", "ULUS-10087", "ULUS-10002", "ULUS-10018",
		"ULES-00125", "ULUS-10081", "ULJM-05062", "ULUS-10040",
		"UCES-00109", "ULUS-10068", "ULJM-05011", "ULUS-10062",
		"UCUS-98641"
	
	
	};
	char *games[] = { 
		"[Hg] Hydrium (A)",
		"007 - From Russia With Love (A)", 
		"007 - From Russia With Love (E)",
		"Armored Core - Formula Front (A)",
		"Armored Core - Formula Front (U)",
		"Armored Core - Formula Front - Extreme Battle (A)",
		"Armored Core - Formula Front - Extreme Battle (E)",
		"Armored Core - Formula Front - International (J)",
		"Bleach - Heat the Soul (A)",
		"Bleach - Heat the Soul (PSP the Best) (J)",
		"Bleach - Heat the Soul 2 (A)",
		"Boku no Watashi no Katamari Damacy (A)",
		"Bokujou Monogatari - Harvest Moon Boy and Girl (A)",
		"Bomberman - Bakufuu Sentai Bombermen (A)",
		"Breath of Fire III (A)",
		"Burnout Legends (A)",
		"Byte Hell 2000 (A)",
		"Capcom Classics Collection Remixed (A)",
		"Coded Arms (PSP the Best) (J)",
		"Coded Gun (K)",
		"Comic Party Portable (Limited Edition) (J)",
		"Crash Tag Team Racing (E)",
		"Crash Tag Team Racing (F)",
		"Dead to Rights - Reckoning (E)",
		"Densha de Go! Pocket - Chuuousenhen (A)",
		"Densha de Go! Pocket - Chuuousenhen (J)",
		"Densha de Go! Pocket - Osaka Kanjousen-Hen (A)",
		"Densha de Go! Pocket - Osaka Kanjousen-Hen (J)",
		"Densha de Go! Pocket - Yamate-Sen-Hen (A)",
		"Densha de Go! Pocket - Yamate-Sen-Hen (J)",
		"Derby Time (PSP the Best) (J)",
		"Dokodemo Issho (PSP The Best) (J)",
		"Dragon Ball Z - Shin Budokai (K)",
		"Dynasty Warriors (F)",
		"Dynasty Warriors (G)",
		"Exit (A)",
		"Exit (J)",
		"Exit (K)",
		"FIFA 06 (E)",
		"FIFA Soccer (A)",
		"FIFA Soccer (K)",
		"FIFA Soccer 06 (A)",
		"FIFA Street 2 (A)",
		"Fight Night Round 3 (A)",
		"Formula One 2005 Portable (A)",
		"Gai - Unmei no Kousaten (J)",
		"Gamble Con Fight (A)",
		"Geki Sengoku Musou (A)",
		"Gensou Suikoden I & II (A)",
		"Gradius Portable (A)",
		"Gradius Portable (K)",
		"Grand Theft Auto - Liberty City Stories (G)",
		"GripShift (K)",
		"Guilty Gear XX #Reload (A)",
		"Harry Potter and the Goblet of Fire (A)",
		"Harukanaru Toki no Nakade - Iroetebako (Koei the Best) (J)",
		"Honkaku Yotarida Mahjong - Mahjong-Ou Portable (J)",
		"Hot Shots Golf - Open Tee (A)",
		"I.Q. Mania (A)",
		"Infected (A)",
		"Initial D - Street Stage (A)",
		"Innocent Life - Shin Bokujou Monogatari (A)",
		"Jet do Go! Pocket (A)",
		"Jet do Go! Pocket (J)",
		"Jui - Dr. Touma Jotarou (J)",
		"Karakuri (A)",
		"Kazuo (J)",
		"Kollon (K)",
		"Koloomn (E)",
		"Kotoba no Puzzle Mojipittan Daijiten (PSP the Best) (J)",
		"Koukaku Kidoutai - Stand Alone Complex - Territory of Hunter (J)",
		"Kurii Mushi Chu~mokan Nagara Iroiro Gochagocha Genttemasu Kedomo... Warai no Tamago L Size (J)",
		"KuruKuru Chameleon (C)", 
		"Le Seigneur des Anneaux - Tactics (F)",
		"Lemmings (A)",
		"Lumines (K)",
		"Lumines (PSP The Best) (J)",
		"Mahjong Kakutou Club (A)",
		"Mahjong Kakutou Club (PSP the Best) (J)",
		"Mahjong Taikai (Koei the Best) (J)",
		"Marvel Nemesis - Rise of the Imperfects (A)",
		"Marvel Nemesis - Rise of the Imperfects (K)",
		"Mega Man Powered Up (A)",
		"Metal Gear Ac!d (A)",
		"Metal Gear Acid (Konami the Best) (J)",
		"Metal Gear Acid 2 (A)",
		"Minna no Golf Portable (A)",
		"Minna no Golf Portable (K)",
		"Minna no Golf Portable (PSP the Best) (J)",
		"MLB (K)",
		"Mobile Train Simulator - Keisei - Toei Akagusa - Keikyuusen (A)",
		"Mobile Train Simulator + Densha de GO! Tokyo Kyuukou Hen (A)",
		"Monster Hunter Portable (A)",
		"Monster Kingdom - Jewel Summoner (A)",
		"MVP Baseball (A)",
		"MVP Baseball (K)",
		"Nakahara no Hasha - Sangoku Shouseiden (A)",
		"Namco Museum Vol. 2 (A)",
		"NBA Live 06 (A)",
		"NBA Live 06 (F)",
		"NBA Street Showdown (A)",
		"NBA Street Showdown (K)",
		"Need for Speed - Most Wanted 5-1-0 (A)",
		"Need for Speed Underground Rivals (A)",
		"Need for Speed Underground Rivals (PSP The Best) (J)",
		"NFL Street 2 - Unleashed (E)",
		"Ooedo Senryoubako (J)",
		"Payout Poker and Casino (U)",
		"Peter Jackson's King Kong - The Official Game of the Movie (K)",
		"Pinball Classics - The Gottlieb Collection (E)",
		"Pipo Saru Academia 2 (A)",
		"Pipo Saru Academia 2 (C)",
		"Piposaru Academia - Dossari! Sarugee Daizenshuu (A)",
		"Piposaru Academia - Dossari! Sarugee Daizenshuu (PSP the Best) (J)",
		"PoPoLoCrois Monogatari - Pietro Ouji no Bouken (A)",
		"PoPoLoCrois Monogatari - Pietro Ouji no Bouken (PSP the Best) (J)",
		"Power Smash - New Generation (A)",   
		"Princess Crown (A)",   	   
		"Pursuit Force (A)",   
		"Pursuit Force (E)",   
		"Puyo Puyo Fever (A)",   
		"Puzzle Bobble Pocket (A)",   
		"Puzzle Bobble Pocket (J)",   
		"Puzzle Bobble Pocket (Taito Best) (J)",   
		"Puzzle Bobble Pocket (Taito Best) (J)",   
		"Puzzle Bobble Pocket (K)",
		"Rengoku - The Tower of Purgatory (PSP the Best) (J)",
		"Rengoku 2 - The Stairway to H.E.A.V.E.N. (A)",
		"Ridge Racer (A)",
		"Ridge Racer (K)",
		"Ridge Racers (A)",
		"Ridge Racers (A)",
		"Ridge Racers (PSP the Best) (J)",
		"RockMan Dash 2 (A)",
		"Rockman Rockman (A)",
		"Rockman X - Irregular Hunter (A)",
		"Sakura Taisen 1 & 2 (A)",
		"Saru Get You P! (A)",
		"Saru Get You P! (PSP the Best) (J)",
		"Sengoku Cannon - Sengoku Ace Episode III (A)",
		"Shin Megami Tensei - Devil Summoner (A)",
		"Shinseiki Evangelion 2 - Tsukurareshi Sekai - Another Cases (10th Anniversary Memorial Box) (J)",
		"Shin Sangoku Musou (PSP the Best) (J)",
		"Shin Sangoku Musou - 2nd Evolution (A)",
		"Shinseiki Genso - SSII Unlimited Side (A)",
		"Shinten Makai - Generation of Chaos IV - Another Side (A)",
		"Shinten Makai - Generation of Chaos IV - Another Side (IF Collection) (Ji)",
		"Shutokou Battle - Zone of Control (A)",
		"Shutokou Battle (PSP the Best) (J)",
		"Simple 2500 Series Portable Volume 1 - The Table Game (A)",
		"Simple 2500 Series Portable - Vol. 3 - The Doko Demo Suiri (J)",
		"Simple 2500 Series Portable - Vol. 4 - The Unou Drill (A)",
		"Simple 2500 Series Portable - Vol. 6 - The Sensha (A)",
		"Smart Bomb (A)",
		"Space Invaders - Galaxy Beat (A)",
		"Space Invaders Pocket (A)",
		"Space Invaders Pocket (J)",
		"Space Invaders Pocket (K)",
		"Spider-Man 2 (E)",
		"Spider-Man 2 (J)",
		"SSX on Tour (A)",
		"Star Wars Battlefront II (A)",
		"Street Fighter Zero 3 - Double Upper (A)",
		"Taiko no Tatsujin Bataburu (A)",
		"Taito Memories Pocket (A)",
		"Taito Memories Pocket (J)",
		"Taito Memories Pocket (J)",
		"Taito Memories Pocket (K)",
		"Tales of Eternia (A)",
		"Tales of Eternia (PSP the Best) (J)",
		"Talkman (A)", 
		"Talkman (A)", 
		"Talkman (C)",
		"Tenchu - Shinobi Taizen (A)",
		"The Dog - Happy Life (J)",
		"The Lord of the Rings - Tactics (A)",
		"The Lord of the Rings - Tactics (E)",
		"The Sims 2 (A)",
		"Tiger Woods PGA Tour (A)",
		"Tiger Woods PGA Tour 06 (A)",
		"Tokimeki Memorial - Forever With You (A)",
		"Tony Hawk's Underground 2 Remix (E)",
		"Tony Hawk's Underground 2 Remix (F)",
		"Twelve - Sengoku Fengshenden (A)",
		"Ultra Puzzle Bobble (A)",
		"Ultra Puzzle Bobble (J)",
		"Untold Legends - Brotherhood of the Blade (K)",
		"V8 Super Cars Australia 2 (E)",
		"Valkyrie Profile - Lenneth (A)",
		"Vampire Chronicle - The Chaos Tower (PSP The Best) (J)",
		"Viewtiful Joe - Battle Carnival (A)",
		"Win-JPT Japanese Beginners (K)",
		"WipEout Pure (A)",
		"WipEout Pure (K)",
		"World Soccer Winning Eleven 9 - Ubiquitous Edition (A)",
		"World Soccer Winning Eleven 9 - Ubiquitous Edition (K)",
		"World Soccer Winning Eleven 9 - Ubiquitous Edition (PSP the Best) (A)",
		"World Soccer Winning Eleven 9 - Ubiquitous Edition (PSP the Best) (J)",
		"WRC Championship (A)",
		"X-Men Legends II - Rise of Apocalypse (E)",
		"XI Coliseum (A)",
		"Yarudora Portable - Blood The Last Vampire (C)",
		"Yarudora Portable - Sampaguita (C)",
		"Ys - Napishtim no Kou (A)",
		"Zero Shiki Kanjou Sentouki (J)",
		"Fifa Street 2 (U)",
		"Burnout Legends (U)",
		"Star Wars Battlefront II (U)",
		"Daxter (U)",
		"Twisted Metal Head-on",
		"Tales of Eternia (E)",
		"The Legend of Heros (U)",
		"Ghost in the Shell (U)",
		"Infected (U)",
		"Socom Fire Team Bravo (U)",
		"Grand Theft Auto - Liberty City Stories (U)",
		"Kingdom of Paradise (U)",
		"Burnout Legends (U)",
		"Dynasty Warriors (U)",
		"Need For Speed Underground Rivals (U)",
		"Coded Arms (U)",
		"Untold Legends Brother Hood of the Blade (U)",
		"Wipeout Pure (U)",
		"Smart Bomb (U)",
		"Hot Shots Open Tee (U)",
		"Ridge Racer (U)",
		"Ys Ark of Napishtim (U)",
		"Untold Legends The Warrior's Code (U)",
		"Metal Gead Ac!d 2 (U)",
		"Splinter Cell Essentials (U)",
		"Metal Gead Ac!d (U)",
		"Midnight Clud 3 Dub (E)",
		"Grand Theft Auto - Liberty City Stories (E)",
		"Wipeout Pure (E)",
		"Tokobot (U)",
		"Tony Hawks Underground 2 Remix (U)",
		"Prince of Persia Revalations (U)",
		"Kao Challengers (E)",
		"SSX On Tour (U)",
		"NBA Street Showdown (E)",
		"Breath of Fire 3 (E)",
		"Frantix (E)",
		"Crash Tag Team Racing (U)",
		"Exit (JN)",
		"Megaman Powered Up (U)",
		"Viewtiful Joe (U)",
		"Lumines (U)",
		"PoPoLoCrois (U)",
		"Burnout Legends (E)",
		"Dragon Ball Z Shin Budokai (U)",
		"Gripshift (U)",
		"Lemmings (E)",
		"Megaman Maverick Hunter X (U)",
		"Puzzle Bobble (J)",
		"Street Fighter Alpha 3 MAX (U)",
		"Syphon Filter Dark Mirror (U)"
	};
	Image* ourImage;
	initGraphics();
	sprintf(buffer, "disc0:/PSP_GAME/PIC1.PNG");
	ourImage = loadImage(buffer);
	if(!ourImage) {
		printf("Image loading failed...\n");
	} else {
		sceDisplayWaitVblankStart();
		blitAlphaImageToScreen(0, 0, 480, 272, ourImage, x, y);
		printTextScreen(0, 0, "LinuxLabs: PSP - http://www.linuxlabs.co.uk", red);
		printTextScreen(0, 10, "UMD Identifier v0.1 by harleyg", red);
		printTextScreen(0, 230, "If you game is not displayed, please post your UMD ID", red);
		printTextScreen(0, 240, "on the 'UMD Codes' thread on PSPUpdates", red);
		printTextScreen(0, 260, "Thanks to: cppnewbie & PSPUpdates", red);
		FILE *f=fopen("disc0:/UMD_DATA.BIN", "r");
		k[fread(k, 1, 10, f)]=0;
		fclose(f);
		printTextScreen(0, 30, k, blue);
		for(i=0; i<257; i++) {
			if(strcmp (k, codes[i]) == 0)
				printTextScreen(0, 40, games[i], blue);
		
	}
	flipScreen();
	}
	sceKernelSleepThread();
	return 0;
}	
