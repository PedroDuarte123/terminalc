#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_CMD 1024

int main() {
    char comando[MAX_CMD];

    while (1) {
        printf("terminal> ");
        fflush(stdout);

        // Lê o comando do usuário
        if (fgets(comando, sizeof(comando), stdin) == NULL) {
            break;
        }

        // Remove o '\n' do final
        comando[strcspn(comando, "\n")] = '\0';

        // Se for "exit", termina o terminal
        if (strcmp(comando, "exit") == 0) {
            break;
        }

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork falhou");
            exit(1);
        } else if (pid == 0) {
            // Estamos no processo filho
            printf("[FILHO] PID: %d | PPID: %d | Executando comando: %s\n",
                   getpid(), getppid(), comando);
            execlp(comando, comando, (char *)NULL); // Substitui o processo atual pelo comando digitado.
            perror("[FILHO] Erro ao executar comando");
            exit(1);
        } else {
            // Estamos no processo pai
            printf("[PAI]   PID: %d | Filho criado: %d\n", getpid(), pid);
            int status;
            waitpid(pid, &status, 0); // Espera o filho terminar e captura seu status de saída.
            printf("[PAI]   Filho %d terminou com status %d\n", pid, status);
        }
    }

    printf("Encerrando terminal.\n");
    return 0;
}
