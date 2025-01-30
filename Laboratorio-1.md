---
title: Laboratório 1

---

Laboratório 1
Thiago Rocha Duarte

<h1>Questão 1.</h1>

```
for (int i=0; i<MAX; i++){
    sum+=i;
}
```

Dividir o intervalo [0, MAX-1] em p partes. (MAX / p)
$$
T=\lfloor\frac{MAX}{p}\rfloor
$$
Cada nó de processamento n vai somar os números em seu próprio intervalo [my_min, my_max].
Distribuir o resto (MAX % p) entre os primeiros nós para garantir uma distribuição equilibrada.
Usar o min() para não passar o valor de MAX.

Valor inicial do nó:
```
my_min = n * T + min(n, MAX % p)
```
Valor final do nó:
```
my_max = (n + 1) * T + min(n + 1, MAX % p) - 1
```

Cada nó executa:
```
soma_parcial = 0;
for (int i = my_min; i <= my_max; i++) {
    soma_parcial += i;
}
```

Após calcular as somas parciais, somar todos os resultados parciais e obter a soma total!

<h1>Questão 2.</h1>

```
for (int i = 0; i < MAX; i++) {
    x = Compute_value(i);
    sum += x;
}
```

Desenvolver uma estratégia para paralelizar um loop onde o tempo de processamento aumenta linearmente com o índice.

- Dividir o loop em partes iguais resultaria em desbalanceamento de carga.

- Iterações com i menor são mais rápidas que iterações com i maior.

Pode fazer um balanceamento dinâmico.

A solução que torna isso possível seria uma fila compartilhada de tarefas, que está disponível para todas as threads ou nós de processamento. Assim, cada thread retira uma tarefa da fila quando estiver pronta para processar. Executa, e então busca a próxima disponível. Isso ocorre até que todas as tarefas na fila sejam concluídas. Tarefas em processamento ou já finalizadas são removidas da fila.

Esse processo promove um equilíbrio natural na distribuição do trabalho, pois threads mais rápidas naturalmente processarão mais tarefas, enquanto as mais lentas processarão menos, sem necessidade de cálculos complexos de distribuição. Também é adaptável à variações no tempo de processamento e ao desempenho de cada thread.

<h1>Questão 3.</h1>

Soma em árvore (Sum Tree Like Reduction).

Dicas:
- Utilize uma variável divisor para determinar se um core deve enviar sua soma ou receber e somar. O divisor deve começar com o valor 2 e dobrar a cada iteração.
- Utilize a variável core_difference para determinar qual é o core com o qual o core corrente deve se comunicar. Ele deve começar com o valor 1 e também dobrar a cada iteração.
- Por exemplo: na iteração 0, 0%divisor = 0 e 1%divisor = 1, logo o core 0 recebe e acumula e o 1 envia.
- Além disso, na iteração 0, 0 + core_difference = 1 e 1 - core_difference = 0, então os cores 0 e 1 se comunicam na primeira iteração.

```
num_cores = potência de 2
divisor = 2
core_difference = 1

Para cada iteração i de 0 até log2(num_cores) - 1:
    Para cada core c de 0 até num_cores - 1:
        if c % divisor == 0:
            # Este core recebe e acumula
            aux = c + core_difference
            if aux < num_cores:
                receive_and_add_from(aux)
        else if c % divisor == core_difference:
            # Este core envia
            aux = c - core_difference
            send_to(aux)
    
    divisor *= 2
    core_difference *= 2
```

- Loop externo executa log2(num_cores) vezes, pois em cada iteração reduzimos pela metade o número de cores ativos.
- Para cada core:
    - Se `c % divisor == 0`, o core recebe e acumula.
    - Se `c % divisor == core_difference`, o core envia.
- Comunicação:
    - Para receber: `c + core_difference`
    - Para enviar: `c - core_difference`
    - Após cada iteração, os valores de `divisor` e `core_difference` são dobrados.

Assim, em cada iteração, metade dos cores ativos enviam seus resultados para a outra metade, reduzindo pela metade o número de cores ativos a cada passo... até que reste apenas um core com a soma total.