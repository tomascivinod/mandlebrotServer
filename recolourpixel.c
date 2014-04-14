//recolourpixel.c
//written by Tomas Donovic
//to get rgb values, just use the following code
/* 
if(returnVar < 255){
	r = returnVar;
}else{
	if(returnVar < 510){
		r = 255
		g = returnVar - 255;
	}else{
		r = 255;
		g = 255;
		b = returnvar - 510;
	}
}	
*/

}

string recolourpixel(int freq){
	int r = 0;
	int g = 0;
	int b = 0;

	if((freq - 85) < 85){
		r = (freq / 85)*255;
	}else{
		if((freq - 170) < 170{
			r = 255;
			g = (freq - 85)*255;
		}else{
			r = 255;
			g = 255;
			b = (freq - 170)*255;
		}
	}
	int returnVar = r + g + b;
	return returnVar;
}