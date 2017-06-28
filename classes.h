class Naves{   
public:
    int x;
    int y;
    int vidas;
    int velocidade;
    int borda_x;
    int borda_y;
    int pontos;
    bool ativo;
    bool tutorial;
    int lateralidadeInimigo;
    void Naves::InitNave(int x, int y, int vidas, int velocidade, int borda_x, int borda_y, int pontos);  
    void Naves::InitInimigoLateral(int Velocidade);
    void Naves::LiberaInimigoLateral(int TELA_LARGURA, int TELA_ALTURA, int chanceLateral);
    void Naves::AtualizarInimigoLateral(int TELA_LARGURA, int TELA_ALTURA, int contador); 
    void Naves::MoveNaveCima();   
    void Naves::MoveNaveBaixo(int TELA_LARGURA, int TELA_ALTURA);
    void Naves::MoveNaveDireita(int TELA_LARGURA, int TELA_ALTURA);
    void Naves::MoveNaveEsquerda();
    void Naves::DesenhaNave(char imagemNave[]);
    void Naves::DesenhaLife(char imagemNave[]);
    int Naves::getValX();      
    int Naves::getValY();
};

class Elementos{
public:
    int x[20];
    int y[20];
    int velocidade[20];
    int borda_x[20];
    int borda_y[20];
    int hp[20];
    bool ativo[20];
    int escolhaElemento[20];
    bool lado[5]; 
    void Elementos::InitElementos(int Borda_x, int Borda_y, int Velocidade); 
    void Elementos::AtualizarBalas(int desativar);
    void Elementos::AtiraBalas(int contador, int jogadorX, int jogadorY); 
    void Elementos::AtualizarBalasInimigoLateral(int desativar, int TELA_ALTURA);
    void Elementos::AtiraBalasInimigoLateral(int contador, int inimigoX, int inimigoY, int jogadorY, bool inimigoAtivo, int lateralidadeInimigo);
    void Elementos::AtualizarBalasInimigos(int TELA_ALTURA, int desativar);
    void Elementos::AtiraBalasInimigos(int inimigoX[5], int inimigoY[5], bool inimigoAtivo[5]);  
    void Elementos::AtualizarInimigos(int TELA_ALTURA, int desativar);
    void Elementos::LiberaMeteoros(int TELA_LARGURA, int HP, int chance);
    void Elementos::LiberaNaves(int TELA_LARGURA, int HP, int chance);
    void Elementos::DesenhaElementos(char imagemNave[], int bordaX, int bordaY);
    void Elementos::DesenhaMeteorosNaves(char img1[], char img2[], char img3[], char img4[], char img5[], int bordaX, int bordaY);              
};

class Particulas{
private:
    int x[100];
    int y[100];
    int velocidade[100];
public:
    void Particulas::InitEstrelas(int TELA_LARGURA, int TELA_ALTURA, int Velocidade);
    void Particulas::AtualizarEstrelas(int TELA_ALTURA);
    void Particulas::DesenhaEstrelas();                
};





