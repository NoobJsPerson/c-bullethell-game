#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
typedef char Map[7][8];
Map map;
int place = 3, layer = 0;
int bullet[2];
_Bool didFire = 0;
void *playerLoop(void *nothing){
	char action;
	scanf("%s",&action);
	switch(action){
		case 'd':
			if(place != 6){
				map[6][place] = '_';
				map[6][++place] = '^';
			}
			break;
		case 'a':
			if(place != 0){
				map[6][place] = '_';
				map[6][--place] = '^';
			}
			break;
		case 's':
			if(didFire) break;
			bullet[0] = 5;
			bullet[1] = place;
			didFire = 1;
	}
	return playerLoop(nothing);
}
void updateScene(){
	static int timer = 0;
	static int timerInterval = 10;
	static int timer2 = 0;
	printf("\033c");
	if(timer == timerInterval)
	{
		for(int i = 0; i < 7; i++){
			int dlayer = layer;
			while(dlayer != 0 && map[dlayer-1][i] != '#'){
				dlayer--;
			}
			map[dlayer][i] = '#';
		}
		layer++;
		timer = 0;
		if(timer2 == 10){
			timerInterval--;
			timer2 = 0;
		} else timer2++;
	} else timer++;
	if(didFire){
		if(bullet[0] != -1){
			map[bullet[0]--][bullet[1]] = '|';
			if(bullet[0] != 4) map[bullet[0]+2][bullet[1]] = '_';
		} else {
			map[0][bullet[1]] = '_';
			didFire = 0;
		}
	}
	
	for(int i = 0; i < 7; i++){
		printf("%s",map[i]);
		printf("\n");
	}
	usleep(266666);
	updateScene();
}
int main(){
	pthread_t thread;
	{
		Map dmap = {
		{"_______"},
		{"_______"},
		{"_______"},
		{"_______"},
		{"_______"},
		{"_______"},
		{"___^___"}
	};
		for(int i = 0; i < 7; i++){
			for(int j = 0; j < 7; j++){
				map[i][j] = dmap[i][j];
			}
		}
	}
	pthread_create(&thread,NULL,playerLoop,NULL);
	updateScene();
	pthread_exit(NULL);
	return 0;
}
