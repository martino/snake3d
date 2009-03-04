/*
 * Snake 3d -- render.c
 * 
 * Gestione della renderizzazione
 *
 */

#include "render.h"

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
  gluPerspective(45.0f, ratio, 0.1f, 1000.0f);
  
  glMatrixMode(GL_MODELVIEW);
  
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
}

/*
 * Funzione per visualizzare il menù
 */
void renderMenu(){
  GLchar text[20];
  GLchar *c;
  GLint x = 80, xi = 80, y = 80;
  sprintf(text, "F - Fullscreen\n");
  for(c = text; *c != '\0'; c++){
    glRasterPos2f(xi, y);
    glutBitmapCharacter((int *)programData.font, *c);
    xi = xi + glutBitmapWidth((int *)programData.font, *c);
  }

  xi = x; 
  y = 100;
  sprintf(text, "N - Nebbia\n");
  for(c = text; *c != '\0'; c++){
    glRasterPos2f(xi, y);
    glutBitmapCharacter((int *)programData.font, *c);
    xi = xi + glutBitmapWidth((int *)programData.font, *c);
  }
	
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
  glLightfv(GL_LIGHT0, GL_AMBIENT,  worldData.ambientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  worldData.diffuseLight);	
  glLightfv(GL_LIGHT0, GL_SPECULAR, worldData.specularLight);
  glLightfv(GL_LIGHT0, GL_POSITION, worldData.positionA);
  /* test spotlight */
  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60.0f);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, worldData.spotDirA);
  /* abilito la luce */
  glEnable(GL_LIGHT0);

  glLightfv(GL_LIGHT1, GL_AMBIENT,  worldData.ambientLight);
  glLightfv(GL_LIGHT1, GL_DIFFUSE,  worldData.diffuseLight);	
  glLightfv(GL_LIGHT1, GL_SPECULAR, worldData.specularLight);
  glLightfv(GL_LIGHT1, GL_POSITION, worldData.positionB);
  /* test spotlight */
  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0f);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, worldData.spotDirB);
  /* abilito la luce */
  glEnable(GL_LIGHT1);


}

/*
 * Funzione per creare la display list del mondo
 */
void createWorld(){
  GLfloat x = WORLDIM, y = WORLDIM, z = WORLDIM;

  glNewList(worldData.wall,GL_COMPILE);
   /*retro*/
   glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -10.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-x, -y, z);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( x, -y, z);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( x,  y, z);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-x,  y, z);

   glEnd();
  
   /*fronte*/
   glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 10.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-x, -y, -z);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( x, -y, -z);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( x,  y, -z);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-x,  y, -z);
   glEnd();
   
   /*latoSX*/
   glBegin(GL_QUADS);
    glNormal3f(10.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-x, -y, -z);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-x, -y,  z);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-x,  y,  z);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-x,  y, -z);
   glEnd(); 

   /*latoDX*/
   glBegin(GL_QUADS);
    glNormal3f(-10.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x, -y, -z);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x, -y,  z);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x,  y,  z);
    glTexCoord2f(0.0f, 1.0f);
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
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( x, y, -z);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( x, y,  z);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-x, y,  z);
   glEnd();

  glEndList();
}

/*
 * Funzione per disegnare il mondo
 */
void drawWorld(){
  // richiama le varie list in base al colore della texture i wall
  glBindTexture(GL_TEXTURE_2D, worldData.cColor);
  glCallList(worldData.wall);
  //soffitto
  //  glBindTexture(GL_TEXTURE_2D, worldData.texObj[TS]);
  glCallList(worldData.sky);
  // ed infine il pavimento
  glBindTexture(GL_TEXTURE_2D, worldData.texObj[TG]);
  glCallList(worldData.ground);







}




/*
 *  questa funzione ha il compito di disegnare il tutto
 *
 * riflessi
 * nebbia...
 *
 */
 
void render(){

  /* Inizio la vera e propria funzione di render*/
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glLoadIdentity();

  glPushAttrib(GL_LIGHTING_BIT);

  // Turn off lighting and specify a bright yellow sphere
  glDisable(GL_LIGHTING);
  
  setOrtographicProjection();
  glLoadIdentity();
  renderText(8, 20, programData.fps);
  resetPerspectiveProjection();

  /* nel caso si visualizzasse il menù */
  if(programData.menu){
	setOrtographicProjection();
    glLoadIdentity();
	renderMenu();
	resetPerspectiveProjection();
	/*	glutSwapBuffers();
		return;*/
  }

  // Restore lighting state variables
  glPopAttrib();

  // parte il rendering normale
  
  /* telecamera */
 
  glRotatef(worldData.angleMX, 1.0f, 0.0f, 0.0f);
  glRotatef(worldData.angleMY, 0.0f, 1.0f, 0.0f);
  glTranslatef(worldData.x, worldData.y, -worldData.z);
  

  lightWorld();
  drawWorld();



  glutSwapBuffers();

}
