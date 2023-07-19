#include <stdio.h>
#include <curl/curl.h>

// Fonction de rappel pour écrire les données de la réponse HTTP dans une chaîne.
size_t write_callback(char *ptr, size_t size, size_t nmemb, char *data) {
    size_t total_size = size * nmemb;
    printf("%.*s", total_size, ptr);
    return total_size;
}

int main() {
    CURL *curl;
    CURLcode res;

    // Initialisation de la bibliothèque curl.
    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Erreur lors de l'initialisation de curl.\n");
        return 1;
    }

    // Définition de l'URL de la requête GET.
    const char *url = "https://www.example.com";

    // Configuration de la requête curl.
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

    // Exécution de la requête GET.
    res = curl_easy_perform(curl);

    // Vérification des erreurs éventuelles.
    if (res != CURLE_OK) {
        fprintf(stderr, "Erreur lors de la requête curl : %s\n", curl_easy_strerror(res));
    }

    // Nettoyage et libération des ressources.
    curl_easy_cleanup(curl);

    return 0;
}
