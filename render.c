/*
 * Snake 3d -- render.c
 * 
 * Gestione della renderizzazione
 *
 */

#include "render.h"

void fog(GLint flag){
  GLfloat fFog[] = {0.25f, 0.25f, 0.25f, 1.0f};

  if(flag){
    	/*
	 * glFog serve per impostare i vari parametri della nebbia
	 *  il primo parametro può essere:
	 *      * GL_FOG_MODE	specifica l'equazione della nebbia:
	 *				GL_LINEAR
	 *				GL_EXP
	 *				GL_EXP2
	 *	* GL_FOG_INDEX		specifica il fog index??
	 *	* GL_FOG_COLOR		specifica il colore della nebbia
	 *    --> con GL_EXP e GL_EXP2
	 *	* GL_FOG_DENSITY	specifica la densità della nebbia, il valore iniziale è 1 e deve essere positivo
	 *    --> con GL_NORMAL
	 *	* GL_FOG_START		specifica dove inizia la nebbia (default 0)
	 *	* GL_FOG_END		specifica dove termina la nebbia (default 1)
	 */
	glFogfv(GL_FOG_COLOR, fFog);		
	glFogf(GL_FOG_DENSITY, 0.01f);
	glFogi(GL_FOG_MODE, GL_EXP2);


    glEnable(GL_FOG);
  }else{
    glDisable(GL_FOG);
  }
}

void changeSize(GLint w, GLint h){
  GLfloat ratio = 0.0f;
  if(h==0) h = 1;

  programData.width = w;
  programData.height = h;
  
  ratio = 1.0 * w / h;
  
  glutWarpPointer(w/2, h/2);
  glutSetCursor(GLUT_CURSOR_NONE);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  /* specifica le dimensioni del viewport*/
  glViewport(0, 0, w, h);
  /* imposta la prospettiva 
   *   gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
   *      fovy   -> angolo di visuale verticale
   *      aspect -> proporzione tra larghezza ed altezza
   *      zNear  -> distanza minima visibile
   *      zFar   -> distanza massima visibile
   */
  gluPerspective(45.0f, ratio, 0.3f, 1000.0f);
  
  glMatrixMode(GL_MODELVIEW);
  glutWarpPointer(programData.width/2, programData.height/2);
  
}

/*
 * Funzione per impostare le proiezioni ortogonali
 */
void setOrtographicProjection(){
  /* torno al projection mode */
  glMatrixMode(GL_PROJECTION);
  /* salvo la matrice */
  glPushMatrix();
  /* carico l'identita' */
  glLoadIdentity();
  /* imposto le proiezioni ortogonali 2d */
  /* la funzione gluOrtho2d ha come parametri:
   *   - left, right  ->  specifica le coordinate dei lati sx e dx
   *   - top, bottom  ->  specifica le coordinati dei lati superiore ed inferiore
   */
  gluOrtho2D(0, programData.width, 0, programData.height);
  /* inverte l'asse y in modo che sia positivo verso il basso */
  glScalef(1, -1, 1);
  /* sposta l'origine nell'angolo in alto a sinistra */
  glTranslatef(0, -programData.height, 0);
  glMatrixMode(GL_MODELVIEW);
  
}


/*
 * Funzione per ritornare alle proiezioni prospettiche
 */
void resetPerspectiveProjection(){
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}


/*
 * Funzione per scrivere del testo
 */

void renderText(float x, float y, char *string){
  char *c;
  int xi = x;
  setOrtographicProjection();
  glLoadIdentity();
  glDisable(GL_TEXTURE_2D);
  glColor3f(0.0f, 0.0f, 0.0f);
  for(c=string; *c != '\0'; c++){
    /* la funzione glRasterPos2 mi permette di specificare la posizione
       x e y di un oggetto */
    glRasterPos2f(xi, y);
    /* 
     * la funzione glutBitmapCharacter disegna un carattere bitmap utilizzando opengl
     *   - font      -> font da utilizzare
     *   - character -> font da stampare
     *
     * Font puo' essere:
     *   - GLUT_BITMAP_8_BY_13
     *   - GLUT_BITMAP_9_BY_15 
     *   - GLUT_BITMAP_TIMES_ROMAN_10 
     *   - GLUT_BITMAP_TIMES_ROMAN_24 
     *   - GLUT_BITMAP_HELVETICA_10
     *   - GLUT_BITMAP_HELVETICA_12
     *   - GLUT_BITMAP_HELVETICA_18
     */
    glutBitmapCharacter((int *)programData.font,*c);
    /* se eventualmente serve uno spazio utilizzo la glutBitmapWidth che mi 
     * restituisce la larghezza del font ed in caso aggiungo dello spazio
     */
    xi = xi + glutBitmapWidth((int *)programData.font, *c); // + spacing;
  }
  glColor3f(1.0f, 1.0f, 1.0f); 
  glEnable(GL_TEXTURE_2D);
  resetPerspectiveProjection();
  
}

/*
 * Funzione che visualizza la mappa di gioco
 */
void renderMap(){
  GLint x=10, y, dim=120, bdim = 2; // variabili per la gestione della mappa
  GLfloat xw,xb,yw,yb,dw=WORMDIA*2,db = DIA*2; // gestione del verme e della palla
  GLfloat xrb=0, yrb=0, xrw=0, yrw=0;
  GLfloat maxD,currD;
  GLfloat bcolor=0.0f;
  WSphere *ite = myWorm.head;
  
  /* rapporto le dimensioni del verme e palla rispetto alla mappa */
  db = db*(dim-db)/WORLDIM;
  dw = dw*(dim-db)/WORLDIM;

  /* inizializzo le variabili */
  y=winHeight-dim-10;
  xw = x+db/2;
  xb = x+db/2;
  yw = y+db/2;
  yb = y+db/2;

  /* 
   * calcolo l'intensita` del colore per la palla 
   *  nero -> distante   rosso -> vicino
   */

  maxD = WORLDIM - dw - db;
  currD = sqrt(dist2Point(-((myWorm.head)->x), -((myWorm.head)->y), (myWorm.head)->z, ball.x, ball.y, ball.z))-dw-db/2;
  bcolor = 1-(currD)/(maxD);
    
  setOrtographicProjection(); 
  glLoadIdentity();
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_POINT_SMOOTH);    
  
  if(ite->y < ball.y*-1.0f){
    /* verme */
    glColor3f(0.0f, 1.0f, 0.0f); 
    glPointSize(dw);
    while(ite != NULL){
      yrw = (((ite->x + WORLDIM )*(dim-db))/(WORLDIM*2));
      xrw = (((ite->z + WORLDIM )*(dim-db))/(WORLDIM*2));
      glBegin(GL_POINTS);
      glVertex2d(xrw+xw,yrw+yw);
      glEnd();
      ite = ite->prev; 
    } 

    /* palla */
    yrb = ((((ball.x*-1.0f)+ WORLDIM )*(dim-db))/(WORLDIM*2)); 
    xrb = ((((ball.z* 1.0f)+ WORLDIM )*(dim-db))/(WORLDIM*2)); 
    switch(ball.texture){
    case 0:
      glColor3f(bcolor, 0.0f, 0.0f); 
      break;
    case 1:
      glColor3f(0.0f, bcolor, 0.0f); 
      break;
    case 2:
      glColor3f(0.0f, 0.0f, bcolor); 
      break;
    case 3:
      glColor3f(bcolor, bcolor, bcolor); 
      break;
    }


    glPointSize(db);
    glBegin(GL_POINTS);
    glVertex2d(xrb+xb,yrb+yb);
    glEnd();
  }else{
    /* palla */
    yrb = ((((ball.x*-1.0f)+ WORLDIM )*(dim-db))/(WORLDIM*2)); 
    xrb = ((((ball.z* 1.0f)+ WORLDIM )*(dim-db))/(WORLDIM*2)); 
    switch(ball.texture){
    case 0:
      glColor3f(bcolor, 0.0f, 0.0f); 
      break;
    case 1:
      glColor3f(0.0f, bcolor, 0.0f); 
      break;
    case 2:
      glColor3f(0.0f, 0.0f, bcolor); 
      break;
    case 3:
      glColor3f(bcolor, bcolor, bcolor); 
      break;
    }
    glPointSize(db);
    glBegin(GL_POINTS);
    glVertex2d(xrb+xb,yrb+yb);
    glEnd();

    /* verme */
    glColor3f(0.0f, 1.0f, 0.0f); 
    glPointSize(dw);
    while(ite != NULL){
      yrw = (((ite->x + WORLDIM )*(dim-db))/(WORLDIM*2));
      xrw = (((ite->z + WORLDIM )*(dim-db))/(WORLDIM*2));
      glBegin(GL_POINTS);
      glVertex2d(xrw+xw,yrw+yw);
      glEnd();
      ite = ite->prev; 
    } 
  }
  
  glColor3f(0.6f, 0.6f, 0.6f); 
  /* rettangolo della mappa */
  glRectf(x,y, x+dim, y+dim);
  /* bordi della mappa */
  glColor3f(0.0f, 0.0f, 0.0f); 
  glRectf(x-bdim,y-bdim, x, y+dim+bdim);
  glRectf(x+dim,y-bdim, x+dim+bdim, y+dim+bdim);
  glRectf(x,y-bdim, x+dim, y+bdim);
  glRectf(x,y+dim, x+dim, y+dim+bdim);

  glColor3f(1.0f, 1.0f, 1.0f); 
  glEnable(GL_TEXTURE_2D);
  resetPerspectiveProjection();
}

/*
 * Funzione per visualizzare il menù
 */
void renderMenu(){
  GLfloat y = winHeight;
  GLfloat x = winWidth;
  setOrtographicProjection();
  glLoadIdentity();
  glBindTexture(GL_TEXTURE_2D, worldData.texObj[TM]);  
  //  glRectf(x, y, 0,0);
  
  glBegin(GL_QUADS);

  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(0,y);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(x, y);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f( x,  0);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(0,  0);
  
  glEnd();
  resetPerspectiveProjection();
}


/*
 * Funzione per visualizzare la vittoria/sconfitta
 */
void renderStatus(){
  GLchar text[40];
  GLchar *c;
  GLint x = (programData.width/2)-80, xi , y = programData.height/2;
  xi = x;

  setOrtographicProjection();
  glLoadIdentity();
  glDisable(GL_TEXTURE_2D);

  glColor3f(1.0f, 0.505f, 0.0f);
  switch(programData.gameStatus){
  case 0:
    sprintf(text, "HAI PERSO"); /* premi pippo per riniziare*/
    break;
  case 2:
    sprintf(text, "HAI VINTO!!");
    break;
  default:
    sprintf(text, "Ma che ci fai qui??");
    break;
  }
  for(c = text; *c != '\0'; c++){
    glRasterPos2f(xi, y);
    glutBitmapCharacter((int *)GLUT_BITMAP_TIMES_ROMAN_24, *c);
    xi = xi + glutBitmapWidth((int *)GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }

  if(programData.points >= 10000)
    xi = x - 55;
  else
    if(programData.points >= 1000)
      xi = x - 50;
    else
      xi = x - 45;
  y += 25;

  sprintf(text, "Totalizzando %d punti", programData.points); 

  for(c = text; *c != '\0'; c++){
    glRasterPos2f(xi, y);
    glutBitmapCharacter((int *)GLUT_BITMAP_TIMES_ROMAN_24, *c);
    xi = xi + glutBitmapWidth((int *)GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }

  xi = x - 60;
  y += 25;
  sprintf(text, "Premi N per ricominciare");
  for(c = text; *c != '\0'; c++){
    glRasterPos2f(xi, y);
    glutBitmapCharacter((int *)GLUT_BITMAP_TIMES_ROMAN_24, *c);
    xi = xi + glutBitmapWidth((int *)GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }

  glColor3f(1.0f, 1.0f, 1.0f); 
  glEnable(GL_TEXTURE_2D);
  resetPerspectiveProjection();
  glutSwapBuffers();


}


/*
 * Funzione che gestisce le luci
 */
void lightWorld(){

  glEnable(GL_LIGHTING);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, worldData.ambientLight);

  /*
   * imposta i parametri per le luci
   * 
   * il primo parametro è il nome della luce:
   * GL_LIGHT n  -> 0 <= n <= maxlight
   * 
   * il secondo parametro sono i parametri della luce che si imposteranno:
   *  * GL_AMBIENT
   *  * GL_DIFFUSE
   *  * GL_SPECULAR
   *  * GL_POSITION
   *  * GL_SPOT_CUTOFF
   *  * GL_SPOT_DIRECTION
   *  * GL_SPOT_EXPONENT
   *  * GL_CONSTANT_ATTENUATION
   *  * GL_LINEAR_ATTENUATION
   *  * GL_QUADRATIC_ATTENUATION
   * 
   * l'ultimo parametro sono i valori che si impostano
   */
  
  worldData.ambientLight[0] = 0.6f;
  worldData.ambientLight[1] = 0.6f;
  worldData.ambientLight[2] = 0.6f;
  worldData.ambientLight[3] = 1.0f;
  
  worldData.diffuseLight[0] = 0.0f;
  worldData.diffuseLight[1] = 0.0f;
  worldData.diffuseLight[2] = 0.0f;
  worldData.diffuseLight[3] = 1.0f;

  worldData.positionA[0] =  0.0f;
  worldData.positionA[1] =  0.0f;
  worldData.positionA[2] =  0.0f;
  worldData.positionA[3] =  0.0f;

  glLightfv(GL_LIGHT0, GL_AMBIENT,  worldData.ambientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  worldData.diffuseLight);	
  glLightfv(GL_LIGHT0, GL_SPECULAR, worldData.specularLight);
  glLightfv(GL_LIGHT0, GL_POSITION, worldData.positionA);

  /* abilito la luce */
  glEnable(GL_LIGHT0);

}

/*
 * Funzione per creare la display list del mondo
 */
void createWorld(){
  GLfloat x = WORLDIM, y = WORLDIM, z = WORLDIM;
  GLfloat rep = 5.0f;
  GLfloat repsd = 10.0f;
  glNewList(worldData.wall,GL_COMPILE);
   /*retro*/
   glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -10.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-x, -y, z);
    glTexCoord2f(rep, 0.0f);
    glVertex3f( x, -y, z);
    glTexCoord2f(rep, rep);
    glVertex3f( x,  y, z);
    glTexCoord2f(0.0f, rep);
    glVertex3f(-x,  y, z);

   glEnd();
  
   /*fronte*/
   glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 10.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-x, -y, -z);
    glTexCoord2f(rep, 0.0f);
    glVertex3f( x, -y, -z);
    glTexCoord2f(rep, rep);
    glVertex3f( x,  y, -z);
    glTexCoord2f(0.0f, rep);
    glVertex3f(-x,  y, -z);
   glEnd();
  glEndList();

  glNewList(worldData.wallsd, GL_COMPILE); 
   /*latoSX*/
   glBegin(GL_QUADS);
    glNormal3f(10.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-x, -y, -z);
    glTexCoord2f(repsd, 0.0f);
    glVertex3f(-x, -y,  z);
    glTexCoord2f(repsd, repsd);
    glVertex3f(-x,  y,  z);
    glTexCoord2f(0.0f, repsd);
    glVertex3f(-x,  y, -z);
   glEnd(); 

   /*latoDX*/
   glBegin(GL_QUADS);
    glNormal3f(-10.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x, -y, -z);
    glTexCoord2f(repsd, 0.0f);
    glVertex3f(x, -y,  z);
    glTexCoord2f(repsd, repsd);
    glVertex3f(x,  y,  z);
    glTexCoord2f(0.0f, repsd);
    glVertex3f(x,  y, -z);
   glEnd();

  glEndList();


  glNewList(worldData.ground,GL_COMPILE);
  // ground

   glBegin(GL_QUADS);
    glNormal3f(0.0f, 10.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-x, -y, -z);
    glTexCoord2f(10.0f, 0.0f);
    glVertex3f( x, -y, -z);
    glTexCoord2f(10.0f, 10.0f);
    glVertex3f( x, -y,  z);
    glTexCoord2f(0.0f, 10.0f);
    glVertex3f(-x, -y,  z);
   glEnd();
  glEndList();
  
  glNewList(worldData.sky,GL_COMPILE);
  // sky
   glBegin(GL_QUADS);
    glNormal3f(0.0f, -10.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-x, y, -z);
    glTexCoord2f(repsd, 0.0f);
    glVertex3f( x, y, -z);
    glTexCoord2f(repsd, repsd);
    glVertex3f( x, y,  z);
    glTexCoord2f(0.0f, repsd);
    glVertex3f(-x, y,  z);
   glEnd();
  glEndList();

  /*verme*/
  glNewList(worldData.worm, GL_COMPILE);
    gluSphere(worldData.q, WORMDIA, 32, 16);
  glEndList();

  /*palla*/
  glNewList(worldData.ball, GL_COMPILE);
    gluSphere(worldData.q, DIA, 32, 16);
  glEndList();

}



/* 
 * Funzione che disegna il verme
 */
void drawWorm(){
  WSphere *ite = myWorm.head;
  glBindTexture(GL_TEXTURE_2D, worldData.texObj[TW]);
  //  fprintf(stderr, "--- begin verme ---\n");
  while(ite != NULL){
    //    fprintf(stderr,"Palla X %f  Y %f  Z %f \n", ite->x, ite->y, ite->z);
    glPushMatrix();
    glTranslatef(-ite->x, -ite->y, ite->z);
    glCallList(worldData.worm);
    glPopMatrix();
    ite = ite->prev;
  }
  //  fprintf(stderr, "--- end verme ---\n");

}

/*
 * Funzione che disegna la palla
 */
void drawBall(){

  glPushMatrix();

  glTranslatef(ball.x, ball.y, ball.z);
  
/*   glColor3f(1.0f, 1.0f, 1.0f); */

  glBindTexture(GL_TEXTURE_2D, worldData.texObj[ball.texture]);

  if(ball.texture == TMY){
    glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
    glEnable(GL_BLEND);
    /*
     * cambia l'equazione con cui si calcola il blending [si creano degli effetti carini]
     *  GL_FUNC_ADD(default) 	  Cf = (Cs * S) + (Cd* D) 
     *  GL_FUNC_SUBTRACT 	  Cf = (Cs * S) – (Cd* D) 
     *  GL_FUNC_REVERSE_SUBTRACT  Cf = (Cd* D) – (Cs * S) 
     *  GL_MIN 			  Cf = min(Cs, Cd) 
     *  GL_MAX 			  Cf = max(C, C)
     */
    glBlendEquation(GL_FUNC_ADD);

    /*
     * serve per specificare i due fattori moltiplicatori per i colori
     * si possono usare i seguenti valori:
     *  GL_ZERO
     *  GL_ONE
     *  GL_SRC_COLOR
     *  GL_ONE_MINUS_SRC_COLOR
     *  GL_DST_COLOR
     *  GL_ONE_MINUS_DST_COLOR
     *  GL_SRC_ALPHA
     *  GL_ONE_MINUS_SRC_ALPHA
     *  GL_DST_ALPHA
     *  GL_ONE_MINUS_DST_ALPHA
     *  GL_SRC_ALPHA_SATURATE
     *  GL_CONSTANT_COLOR
     *  GL_ONE_MINUS_CONSTANT_COLOR
     *  GL_CONSTANT_ALPHA
     *  GL_ONE_MINUS_CONSTANT_ALPHA
     */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glCallList(worldData.ball);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T); 
    glDisable(GL_BLEND); 
  }else{
    glCallList(worldData.ball);
  }

  glPopMatrix();


}

/*
 * Funzione per disegnare il mondo
 */
void drawWorld(){
  /* disegno le pareti */
  glPushMatrix();

  // richiama le varie list in base al colore della texture i wall
  glBindTexture(GL_TEXTURE_2D, worldData.texObj[worldData.texWall]);
  glCallList(worldData.wall);
  glBindTexture(GL_TEXTURE_2D, worldData.texObj[worldData.texWallSd]);
  glCallList(worldData.wallsd);

  //soffitto
  glCallList(worldData.sky);
  // pavimento
  glBindTexture(GL_TEXTURE_2D, worldData.texObj[TP]);
  glCallList(worldData.ground);
  glPopMatrix();

  /* disegno il verme */
  drawWorm();
  /* disegno la palla */
  drawBall();
  
  
}




/*
 *  questa funzione ha il compito di disegnare il tutto
 *
 * nebbia...
 *
 */
 
void render(){

  /* Inizio la vera e propria funzione di render*/
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glLoadIdentity();

  glPushAttrib(GL_LIGHTING_BIT);

  glDisable(GL_LIGHTING);

  /* visualizzo il menu */
  if(programData.menu){
    renderMenu();
    glutSwapBuffers();
    return;
  }
   
  renderText(8, 20, programData.fps);
  renderText(660, 20, programData.pointsOSD);

  
  switch(programData.gameStatus){
  case 0:
  case 2:
    renderStatus();
    return;
    break;
  default:
    break;

  }

  // Restore lighting state variables
  glPopAttrib();

  // parte il rendering normale
  

  /* telecamera */
  glLoadIdentity();
  glTranslatef(0, 0, myWorm.dia);   
  glRotatef(worldData.angleMX, 1.0f, 0.0f, 0.0f);
  glRotatef(worldData.angleMY, 0.0f, 1.0f, 0.0f);
  glTranslatef((myWorm.head)->x, (myWorm.head)->y, -(myWorm.head)->z);
  //lightWorld();
  lightWorld();
  drawWorld();

  renderMap();
  glutSwapBuffers();

}
