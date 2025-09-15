#include <gtk-3.0/gtk/gtk.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

sqlite3 *db;
char *err_msg = 0;

void botao_inserir_erro_clicked(GtkButton *button, gpointer user_data);
void botao_inserir_done_clicked(GtkButton *button, gpointer user_data);
void botao_inserir_inserir_clicked(GtkButton *button, gpointer user_data);
void botao_inserir_cancelar_clicked(GtkButton *button, gpointer user_data);
void botao_inserir_clicked(GtkButton *button, gpointer user_data);
void botao_janela_remover_done_clicked(GtkButton *button, gpointer user_data);
void botao_janela_remover_remover_clicked(GtkButton *button, gpointer user_data);
void botao_janela_remover_cancelar_clicked(GtkButton *button, gpointer user_data);
void botao_remover_clicked(GtkButton *button, gpointer user_data);

int main(int argc, char *argv[]){
    
    // Abre/Cria o banco de dados 
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

    g_signal_connect(botao_inserir, "clicked", G_CALLBACK(botao_inserir_clicked), NULL);
    g_signal_connect(botao_remover, "clicked", G_CALLBACK(botao_remover_clicked), NULL);
    gtk_builder_connect_signals(builder, NULL);

    gtk_widget_show_all(janela_principal);
    
    gtk_main();

    sqlite3_close(db);
    return 0;
}

//Função inserir (inicio).
bool is_right(int idade, int telefone){
    int count = 0;
    while (telefone > 0){
        telefone = telefone / 10;
        count ++;
    }
    return (idade > 0 && idade < 120) && (count <= 15); 
}

void botao_inserir_erro_clicked(GtkButton *button, gpointer user_data){
    GtkWidget *janela_inserir_erro = GTK_WIDGET(gtk_builder_get_object(user_data, "janela_inserir_erro"));
    gtk_widget_hide(janela_inserir_erro);
}

void botao_inserir_done_clicked(GtkButton *button, gpointer user_data){
    GtkWidget *janela_inserir_done = GTK_WIDGET(gtk_builder_get_object(user_data, "janela_inserir_done"));
    gtk_widget_hide(janela_inserir_done);
}
void botao_inserir_inserir_clicked(GtkButton *button, gpointer user_data){
 
    GtkWidget *janela_inserir = GTK_WIDGET(gtk_builder_get_object(user_data, "janela_inserir"));
    GtkWidget *nome_entry = GTK_WIDGET(gtk_builder_get_object(user_data, "nome_entry"));
    GtkWidget *idade_entry = GTK_WIDGET(gtk_builder_get_object(user_data, "idade_entry"));
    GtkWidget *telefone_entry = GTK_WIDGET(gtk_builder_get_object(user_data, "telefone_entry"));
    GtkWidget *janela_inserir_done = GTK_WIDGET(gtk_builder_get_object(user_data, "janela_inserir_done"));
    GtkWidget *botao_inserir_done = GTK_WIDGET(gtk_builder_get_object(user_data, "botao_inserir_done"));
    GtkWidget *janela_inserir_erro = GTK_WIDGET(gtk_builder_get_object(user_data, "janela_inserir_erro"));
    GtkWidget *botao_inserir_erro = GTK_WIDGET(gtk_builder_get_object(user_data, "botao_inserir_erro"));

    const gchar *nome = gtk_entry_get_text(GTK_ENTRY(nome_entry));
    const gchar *idade = gtk_entry_get_text(GTK_ENTRY(idade_entry));
    const gchar *telefone = gtk_entry_get_text(GTK_ENTRY(telefone_entry));

    // Converte as entradas para inteiro.
    int idade_int = atoi(idade);
    int telefone_int = atoi(telefone);
    bool check = is_right(idade_int, telefone_int);
    if (check){
        char sql[200];
        sprintf(sql, "INSERT INTO clientes (Nome, Idade, Telefone) VALUES ('%s', %d, %d)", nome, idade_int, telefone_int);
        if(sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK){
            printf("Erro: %s", &err_msg);
            sqlite3_free(err_msg);
        }
        gtk_widget_show_all(janela_inserir_done);
        g_signal_connect(botao_inserir_done, "clicked", G_CALLBACK(botao_inserir_done_clicked), user_data);
    }else{
        gtk_widget_show_all(janela_inserir_erro);
        g_signal_connect(botao_inserir_erro, "clicked", G_CALLBACK(botao_inserir_erro_clicked), user_data);
    }
    gtk_widget_destroy(janela_inserir);
}

void botao_inserir_cancelar_clicked(GtkButton *button, gpointer user_data){
    GtkWidget *janela_inserir = GTK_WIDGET(gtk_builder_get_object(user_data, "janela_inserir"));
    gtk_widget_destroy(janela_inserir);
}

void botao_inserir_clicked(GtkButton *button, gpointer user_data){
    GtkBuilder *builder = gtk_builder_new_from_file("interface.ui");
    GtkWidget *janela_inserir = GTK_WIDGET(gtk_builder_get_object(builder, "janela_inserir"));
    GtkWidget *botao_inserir_inserir = GTK_WIDGET(gtk_builder_get_object(builder, "botao_inserir_inserir"));
    GtkWidget *botao_inserir_cancelar = GTK_WIDGET(gtk_builder_get_object(builder, "botao_inserir_cancelar"));
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show_all(janela_inserir);
    g_signal_connect(botao_inserir_inserir, "clicked", G_CALLBACK(botao_inserir_inserir_clicked), builder);
    g_signal_connect(botao_inserir_cancelar, "clicked", G_CALLBACK(botao_inserir_cancelar_clicked), builder);
    
}
//Função inserir (Fim)

// Função remover (Inicio)
void botao_janela_remover_done_clicked(GtkButton *button, gpointer user_data){
     GtkWidget *janela_remover_done = GTK_WIDGET(gtk_builder_get_object(user_data, "janela_remover_done"));
     gtk_widget_destroy(janela_remover_done);  
}
void botao_janela_remover_erro_clicked(GtkButton *button, gpointer user_data){
    GtkWidget *janela_remover_erro = GTK_WIDGET(gtk_builder_get_object(user_data, "janela_remover_erro"));
     gtk_widget_destroy(janela_remover_erro);  
}
void botao_janela_remover_cancelar_clicked(GtkButton *button, gpointer user_data){
    GtkWidget *janela_remover = GTK_WIDGET(gtk_builder_get_object(user_data, "janela_remover"));
    gtk_widget_destroy(janela_remover); 
}

void botao_janela_remover_remover_clicked(GtkButton *button, gpointer user_data){
    GtkWidget *janela_remover = GTK_WIDGET(gtk_builder_get_object(user_data, "janela_remover"));
    GtkWidget *entry_janela_remover = GTK_WIDGET(gtk_builder_get_object(user_data, "entry_janela_remover"));
    GtkWidget *escolha_janela_remover = GTK_WIDGET(gtk_builder_get_object(user_data, "escolha_janela_remover"));
    GtkWidget *janela_remover_done = GTK_WIDGET(gtk_builder_get_object(user_data, "janela_remover_done"));
    GtkWidget *janela_remover_erro = GTK_WIDGET(gtk_builder_get_object(user_data, "janela_remover_erro"));
    GtkWidget *botao_janela_remover_done = GTK_WIDGET(gtk_builder_get_object(user_data, "botao_janela_remover_done"));
    GtkWidget *botao_janela_remover_erro = GTK_WIDGET(gtk_builder_get_object(user_data, "botao_janela_remover_erro"));

    const gchar *info = gtk_entry_get_text(GTK_ENTRY(entry_janela_remover));
    const gchar *escolha = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(escolha_janela_remover));
    int info_int = atoi(info);
    char sql[200];
    if(g_strcmp0(escolha, "ID") == 0){
        sprintf(sql, "DELETE FROM clientes WHERE Id = %d", info_int);
    }else if(g_strcmp0(escolha, "Telefone") == 0){
        sprintf(sql, "DELETE FROM clientes WHERE Telefone = %d", info_int);
    }
    
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    int changes = sqlite3_changes(db);
    if(rc != SQLITE_OK){
        printf("Erro: %s\n", &err_msg);
        sqlite3_free(err_msg);
    }
    if(changes > 0){
        gtk_widget_show_all(janela_remover_done);
        g_signal_connect(botao_janela_remover_done, "clicked", G_CALLBACK(botao_janela_remover_done_clicked), user_data);
    }else{
        gtk_widget_show_all(janela_remover_erro);
        g_signal_connect(botao_janela_remover_erro, "clicked", G_CALLBACK(botao_janela_remover_erro_clicked), user_data);
    }

    gtk_widget_destroy(janela_remover);    
    
}

void botao_remover_clicked(GtkButton *button, gpointer user_data){
    GtkBuilder *builder = gtk_builder_new_from_file("interface.ui");
    GtkWidget *janela_remover = GTK_WIDGET(gtk_builder_get_object(builder, "janela_remover"));
    GtkWidget *botao_janela_remover_remover = GTK_WIDGET(gtk_builder_get_object(builder, "botao_janela_remover_remover"));
    GtkWidget *botao_janela_remover_cancelar = GTK_WIDGET(gtk_builder_get_object(builder, "botao_janela_remover_cancelar"));
    gtk_widget_show_all(janela_remover);
    g_signal_connect(botao_janela_remover_remover, "clicked", G_CALLBACK(botao_janela_remover_remover_clicked), builder);
    g_signal_connect(botao_janela_remover_cancelar, "clicked", G_CALLBACK(botao_janela_remover_cancelar_clicked), builder);
}