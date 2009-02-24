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
  gluPerspective(45.0f, ratio, 0.1f, 200.0f);
  
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
 * Funzione per creare la display list del mondo
 */
void createWorld(){
  GLfloat x = WORLDIM, y = WORLDIM, z = WORLDIM;

  glNewList(worldData.wall,GL_COMPILE);
   /*retro*/

   /*   glBegin(GL_QUADS);
    glVertex3f(-x, -y, z);
    glVertex3f( x, -y, z);
    glVertex3f( x,  y, z);
    glVertex3f(-x,  y, z);
    glEnd();*/
   
   /*fronte*/
   glBegin(GL_QUADS);
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

  // ed infine il pavimento
  glBindTexture(GL_TEXTURE_2D, worldData.texObj[TG]);
  glCallList(worldData.ground);

  // ed infine il soffitto
  glBindTexture(GL_TEXTURE_2D, worldData.texObj[TS]);
  glCallList(worldData.sky);



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
	glutSwapBuffers();
	return;
  }

  // parte il rendering normale
  
  // telecamera
  glTranslatef(0.0f, 0.0f, -100.0f);

  
  drawWorld();
  


  glutSwapBuffers();

}
