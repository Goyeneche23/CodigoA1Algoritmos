#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

string leerArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    string contenido, linea;
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir " << nombreArchivo << endl;
        return "";
    }
    while (getline(archivo, linea)) {
        contenido += linea;
    }
    return contenido;
}

int buscarPatron(const string& texto, const string& patron) {
    if (patron.empty()) return -1;

    int n = texto.size();
    int m = patron.size();

    // arreglo LPS 
    vector<int> lps(m, 0);
    int len = 0;   // longitud prefijo más largo
    int i = 1;     

    while (i < m) {
        if (patron[i] == patron[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1]; // retrocede en patron
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }

    // Buscar el patron usando la tabla LPS
    i = 0;  // indice texto
    int j = 0;  // indice para patron

    while (i < n) {
        if (texto[i] == patron[j]) {
            i++;
            j++;
        }

        if (j == m) {
            // Patron encontrado, devolver posicion
            return i - j;
        } else if (i < n && texto[i] != patron[j]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    return -1; // no encontrado
}


pair<int,int> encontrarPalindromoMasLargo(const string& s) {
    if (s.empty()) return {1, 1};
    string t = "#";
    for (char c : s) { t += c; t += '#'; }
    int n = t.size(), center = 0, right = 0, max_len = 0, max_center = 0;
    vector<int> p(n, 0);
    for (int i = 0; i < n; i++) {
        if (i < right) p[i] = min(right - i, p[2*center - i]);
        while (i-p[i]-1 >= 0 && i+p[i]+1 < n && t[i-p[i]-1] == t[i+p[i]+1]) p[i]++;
        if (i+p[i] > right) { center = i; right = i+p[i]; }
        if (p[i] > max_len) { max_len = p[i]; max_center = i; }
    }
    int inicio = (max_center - max_len) / 2 + 1;
    int fin = inicio + max_len - 1;
    return {inicio, fin};
}

pair<int,int> encontrarSubstringComun(const string& s1, const string& s2) {
    int n = s1.length(), m = s2.length(), max_len = 0, end_index = 0;
    vector<vector<int>> dp(n+1, vector<int>(m+1, 0));
    for (int i=1;i<=n;i++){
        for (int j=1;j<=m;j++){
            if (s1[i-1] == s2[j-1]){
                dp[i][j] = dp[i-1][j-1] + 1;
                if (dp[i][j] > max_len){
                    max_len = dp[i][j];
                    end_index = i-1;
                }
            }
        }
    }
    if (max_len == 0) return {1,0};
    int inicio = end_index - max_len + 2;
    int fin = end_index + 1;
    return {inicio, fin};
}

int main() {
    string transmission1 = leerArchivo("transmission01.txt");
    string transmission2 = leerArchivo("transmission02.txt");
    string mcode1 = leerArchivo("mcode01.txt");
    string mcode2 = leerArchivo("mcode02.txt");
    string mcode3 = leerArchivo("mcode03.txt");

    vector<pair<string,string>> busquedas = {
        {transmission1, mcode1}, {transmission1, mcode2}, {transmission1, mcode3},
        {transmission2, mcode1}, {transmission2, mcode2}, {transmission2, mcode3}
    };

    for (auto &b : busquedas) {
        int pos = buscarPatron(b.first, b.second);
        if (pos != -1) cout << "true " << pos << "\n";
        else cout << "false\n";
    }

    auto p1 = encontrarPalindromoMasLargo(transmission1);
    auto p2 = encontrarPalindromoMasLargo(transmission2);
    cout << p1.first << " " << p1.second << "\n";
    cout << p2.first << " " << p2.second << "\n";

    auto sub = encontrarSubstringComun(transmission1, transmission2);
    cout << sub.first << " " << sub.second << "\n";
}
