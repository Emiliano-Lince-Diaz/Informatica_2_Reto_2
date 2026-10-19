// main.cpp
#include "RedSocial.h"
#include "Menu.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

void asegurarArchivosDataset() {
    ifstream check("usuarios.txt");
    if (check.is_open()) {
        check.close();
        return; // Ya existen los archivos
    }

    cout << "[Inicializador] Generando los 6 archivos .txt con delimitador '|' (1050 usuarios)..." << endl;
    const int totalUsuarios = 1050;

    // 1. usuarios.txt: id|nombreUsuario|fechaNac|correo|foto|pass|ciudad|pais|fechaInsc
    ofstream archU("usuarios.txt");
    for (int i = 0; i < totalUsuarios; i++) {
        archU << i << "|usuario_" << i << "|15/05/2002|usuario_" << i << "@udea.edu.co|"
              << "/users/udeabook/usuario_" << i << "/albums/profile.png|pass123|Medellin|Colombia|01/02/2026\n";
    }
    archU.close();

    // 2. amistades.txt: idA|idB (Conecta rangos para tener <=10, 11..20 y >20 amigos en comun)
    ofstream archA("amistades.txt");
    for (int i = 0; i < totalUsuarios; i++) {
        for (int j = i + 1; j < i + 32 && j < totalUsuarios; j++) {
            if ((i + j) % 3 != 0) {
                archA << i << "|" << j << "\n";
            }
        }
    }
    // Asegurar que usuario_0 y usuario_1 sean amigos directos para las pruebas del video
    archA << "0|1\n";
    archA.close();

    // 3. solicitudes.txt: idRemitente|idReceptor
    ofstream archS("solicitudes.txt");
    archS << "50|0\n";
    archS << "51|0\n";
    for (int i = 60; i < 80; i++) {
        archS << i << "|" << (i + 35) << "\n";
    }
    archS.close();

    // 4. albumes.txt: idUsuario|nombreAlbum|rutaAlbum|privacidad (0=Publico, 1=Amigos, 2=Privado)
    // El video exige visitar un amigo con al menos 2 albumes y 5 fotos en cada album (ej. usuario_1)
    ofstream archAlb("albumes.txt");
    archAlb << "1|Cartagena2026|/users/udeabook/usuario_1/albums/Cartagena2026|0\n";
    archAlb << "1|GraduacionUdeA|/users/udeabook/usuario_1/albums/GraduacionUdeA|1\n";
    archAlb << "1|SecretoPrivado|/users/udeabook/usuario_1/albums/SecretoPrivado|2\n";
    for (int u = 2; u <= 20; u++) {
        archAlb << u << "|Viajes_" << u << "|/users/udeabook/usuario_" << u << "/albums/Viajes_" << u << "|1\n";
    }
    archAlb.close();

    // 5. fotos.txt: idUsuario|nombreAlbum|fechaCaptura|tamano|rutaFoto|likes
    ofstream archFot("fotos.txt");
    for (int f = 1; f <= 5; f++) {
        archFot << "1|Cartagena2026|0" << f << "/09/2026|" << (250 + f * 10)
        << "|/users/udeabook/usuario_1/albums/Cartagena2026/Playa" << f << ".png|" << (f * 3) << "\n";
        archFot << "1|GraduacionUdeA|1" << f << "/09/2026|" << (300 + f * 15)
                << "|/users/udeabook/usuario_1/albums/GraduacionUdeA/Grado" << f << ".png|" << (f * 5) << "\n";
    }
    archFot << "1|SecretoPrivado|20/09/2026|400|/users/udeabook/usuario_1/albums/SecretoPrivado/Priv1.png|0\n";
    for (int u = 2; u <= 20; u++) {
        for (int f = 1; f <= 5; f++) {
            archFot << u << "|Viajes_" << u << "|10/09/2026|200|/users/udeabook/usuario_"
                    << u << "/albums/Viajes_" << u << "/Foto" << f << ".png|2\n";
        }
    }
    archFot.close();

    // 6. mensajes.txt: idEmisor|idReceptor|fecha|leido|texto (Rango > 6 semanas: 01/08/2026 a 15/10/2026)
    ofstream archM("mensajes.txt");
    const char* fechasDemo[8] = {
        "02/08/2026", "12/08/2026", "25/08/2026", "05/09/2026",
        "18/09/2026", "29/09/2026", "08/10/2026", "15/10/2026"
    };
    for (int i = 0; i < 8; i++) {
        archM << "1|0|" << fechasDemo[i] << "|0|Hola usuario_0, mensaje recibido de prueba #" << (i + 1) << "\n";
    }
    for (int i = 0; i < 600; i++) {
        int em = (i % 40) + 1;
        int rec = ((i + 7) % 40) + 1;
        if (em != rec) {
            archM << em << "|" << rec << "|" << fechasDemo[i % 8] << "|" << (i % 2)
            << "|Mensaje general de la red social #" << i << "\n";
        }
    }
    archM.close();

    cout << "[OK] Dataset de 6 archivos generado exitosamente." << endl;
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    asegurarArchivosDataset();

    RedSocial udeaBook;
    Menu menu(&udeaBook);
    menu.ejecutar();

    return 0;
}