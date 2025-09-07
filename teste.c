#include <gtk-3.0/gtk/gtk.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

sqlite3 *db;
char *err_msg = 0;

void botao_inserir_inserir_clicked(GtkButton *button, gpointer user_data);
void botao_inserir_clicked(GtkButton *button, gpointer user_data);

int main(int argc, char *argv[]){
    
    
    sqlite3_open("mydatabase.db", &db);

    char sql[200] = "CREATE TABLE IF NOT EXISTS clientes ("
                    "Id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "Nome TEXT, Idade INTEGER, Telefone INTEGER);";

    if(sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK){
        printf("Erro: %s", err_msg);
        sqlite3_free(err_msg);
    };
    

    gtk_init(&argc, &argv);

    GtkBuilder *builder = gtk_builder_new_from_file("interface.ui");

    GtkWidget *janela_principal = GTK_WIDGET(gtk_builder_get_object(builder, "janela_principal"));
    GtkWidget *botao_inserir = GTK_WIDGET(gtk_builder_get_object(builder, "botao_inserir"));
    GtkWidget *botao_remover = GTK_WIDGET(gtk_builder_get_object(builder, "botao_remover"));
    GtkWidget *botao_listar = GTK_WIDGET(gtk_builder_get_object(builder, "botao_listar"));

    g_signal_connect(botao_inserir, "clicked", G_CALLBACK(botao_inserir_clicked), builder);
    gtk_builder_connect_signals(builder, NULL);

    gtk_widget_show_all(janela_principal);

    gtk_main();

    sqlite3_close(db);
    return 0;
}

void botao_inserir_inserir_clicked(GtkButton *button, gpointer user_data){
 
    GtkWidget *janela_inserir = GTK_WIDGET(gtk_builder_get_object(user_data, "janela_inserir"));
    GtkWidget *nome_entry = GTK_WIDGET(gtk_builder_get_object(user_data, "nome_entry"));
    GtkWidget *idade_entry = GTK_WIDGET(gtk_builder_get_object(user_data, "idade_entry"));
    GtkWidget *telefone_entry = GTK_WIDGET(gtk_builder_get_object(user_data, "telefone_entry"));
    
    const gchar *nome = gtk_entry_get_text(GTK_ENTRY(nome_entry));
    const gchar *idade = gtk_entry_get_text(GTK_ENTRY(idade_entry));
    const gchar *telefone = gtk_entry_get_text(GTK_ENTRY(telefone_entry));
    int idade_int = atoi(idade);
    int telefone_int = atoi(telefone);

    char sql[200];
    sprintf(sql, "INSERT INTO clientes (Nome, Idade, Telefone) VALUES ('%s', %d, %d)", nome, idade_int, telefone_int);
    if(sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK){
        printf("Erro: %s", &err_msg);
        sqlite3_free(err_msg);
    }

    gtk_widget_destroy(janela_inserir);
}

void botao_inserir_clicked(GtkButton *button, gpointer user_data){
    GtkWidget *janela_inserir = GTK_WIDGET(gtk_builder_get_object(user_data, "janela_inserir"));
    GtkWidget *botao_inserir_inserir = GTK_WIDGET(gtk_builder_get_object(user_data, "botao_inserir_inserir"));
    GtkWidget *botao_inserir_cancelar = GTK_WIDGET(gtk_builder_get_object(user_data, "botao_inserir_cancelar"));
    gtk_widget_show_all(janela_inserir);
    g_signal_connect(botao_inserir_inserir, "clicked", G_CALLBACK(botao_inserir_inserir_clicked), user_data);
}