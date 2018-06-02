## Progetto Hybrid MPI-OpenMP Application Autotuning in HPC systems
Il progetto consiste nella creazione di un tool che possa permettere ad applicazioni implementanti le librerie OpenMP e MPI l’autotuning. Ho dovuto identificare i parametri che permettono l’ottimizzazione di un’applicazione ibrida parallela e, mediante test, trovare il loro valore che permetta di ridurre al minimo il tempo di esecuzione.

Per iniziare ho dovuto familiarizzare con le librerie coinvolte: durante i miei studi ho trattato il tema del parallelismo ma non con un approccio rivolto a sistemi HPC. MPI e OpenMP permettono infatti un’implementazione di processi e thread rispettivamente molto più efficiente e forniscono metodi molto comodi per il calcolo parallelo. Per far pratica ho scritto un programma molto semplice che si limita a svolgere il prodotto tra matrici (il valore di ogni elemento per semplicità è dipendente dalla posizione), procedendo poi a renderlo parallelo sia con MPI sia con OpenMP. Dopo aver fatto un paio di esecuzioni di prova, ho notato che per quest’applicazione l’approccio basato su OpenMP senza MPI ha portato i risultati migliori: del resto l’algoritmo implementato prevede un utilizzo dei dati difficile da serializzare e quindi l’approccio basato sui thread (che condividono la memoria) è più efficiente rispetto a quello dei processi (che non lo fanno). Bisogna considerare tuttavia che ho compiuto questo test solo sul mio portatile, dotato di un processore dual-core: il numero di core influenza molto i risultati raggiunti poiché è correlato al numero massimo di esecuzioni in parallelo. Pensavo che il numero di core potesse aiutarmi a ridurre il campo di analisi del problema: le esecuzioni in cui il prodotto tra numero di processi e di thread per processo superava quello dei core del mio PC si rivelavano in media più lente, superando in alcuni casi l’esecuzione non parallela. Questi ritardi erano dovuti sia a overhead per la creazione dei processi, sia ad accodamenti di scheduling. Questa tendenza è rimasta anche nelle esecuzioni successive, con qualche eccezione.

Dopo aver realizzato la mia applicazione ibrida tunabile, sono stato introdotto al progetto mARGOt: si tratta di un tool sviluppato dal Politecnico di Milano per l’ottimizzazione di un qualunque aspetto di un’applicazione. mARGOt permette di estrapolare dal codice una serie di knobs e monitor: agendo opportunamente sui primi si può ottimizzare il valore dei secondi. mARGOt si basa sul principio MAPE-K, cioè Monitor, Analyze, Plan ed Execute basato sulla Knowledge: secondo dei dati già raccolti permette di scegliere la configurazione che meglio si adatta al problema. Il tool richiede di modificare il codice in modo da notificare quando bisogna iniziare e finire le misurazioni. Questo poteva diventare un altro problema, poiché di solito il codice per i sistemi HPC è eseguito da persone specializzate in altri ambiti e che quindi potrebbero far fatica a compiere le giuste modifiche al codice. Inoltre potrebbe essere necessario ottimizzare applicazioni non open-source, rendendo la modifica difficile e potenzialmente causa d’instabilità.

Ho ottenuto la soluzione cercando di rendere il problema più riutilizzabile: non è molto utile un programma in grado di ottimizzare l’esecuzione di un prodotto di matrici (bisognerebbe in primis ottimizzare l’algoritmo, vedesi l’algoritmo di Strassen), tuttavia è interessante un tool in grado di ottimizzare l’esecuzione di un qualunque programma. Ho quindi disaccoppiato il programma da mARGOt, creando una sorta di wrapper. Questo si occupava sia dell’esecuzione sia della compilazione del codice, più tardi ho deciso di separare i due aspetti, per migliorare la leggibilità del codice.

mARGOt si basa sulla Knowledge e definisce la sintassi che essa deve avere: non mette a disposizione tuttavia i mezzi per ottenerla. Serviva dunque un tool in grado di estrapolare i dati dal programma, cioè testare tutte le possibili configurazioni dei knob e tener traccia dei valori dei monitor. Adattando un po’ una copia del codice del wrapper ho creato agorizer (il nome deriva da AGORA’, di cui parlerò più avanti). Questo tool è accompagnato da un suo file di configurazione che permette di decidere i valori dei knob e il numero di ripetizioni per punto operativo. Per ogni esecuzione monitora il tempo impiegato e ne calcola una media. Raccoglie tutti i dati e crea un file con sintassi compatibile con mARGOt.

L’esecuzione di agorizer è tuttavia molto dispendiosa in termini di tempo e risorse: dovendo analizzare ogni configurazione possibile molte volte, finisce per essere un costo sopportabile solo se l’esecuzione del programma tunato avverrà ripetutamente. La mole di dati potrebbe essere inoltre scomoda da gestire, e quindi è forse meglio adottare un approccio alternativo. AGORA’ è un plugin di mARGOt basato su Cassandra che permette l’estrapolazione della Knowledge da remoto. L’utilità di interfacciarsi con il remoto può essere molta: avendo a disposizione una maggiore capacità di calcolo, è possibile pensare a un algoritmo più efficiente per l’analisi dei dati, che magari permetta di evitare l’esecuzione di alcuni casi chiaramente non efficienti. Quest’algoritmo purtroppo per ora non è presente, quindi non si avrebbero guadagni di tempo nell’utilizzo di AGORA’; inoltre si genererebbero delle perdite di tempo dovute alle trasmissioni dati da e a remoto. Il supporto con AGORA’ è [non ancora ma tra poco] comunque garantito.

I knob utilizzati per i vari test sono principalmente due: numero di thread e di processi. OpenMP fornisce anche la possibilità di scegliere la posizione o una politica di disposizione dei thread. Sfruttando questa funzionalità ho aggiunto un terzo knob introducendo quattro casi, che poi saranno mappati nei corrispettivi valori testuali della variabile d’ambiente OMP_PROC_BIND:

1.	SPREAD, i thread sono sparsi lungo la partizione di memoria in uso;
2.	CLOSE, i thread sono vicini tra loro;
3.	MASTER, i thread sono più vicini possibile al thread principale;
4.	FALSE, la gestione della posizione dei thread è lasciata al sistema. 

Unendo tutti questi elementi otteniamo il progetto AGATHA (acronimo di AGAtha Tunes Hybrid Applications). In esso ho posto qualche script per aiutare l’utente nello svolgimento dei vari passaggi per l’autotuning. L’utente può inoltre configurare completamente i valori entro cui agorizer opera, permettendogli di scegliere tra precisione, velocità e copertura come meglio preferisce. Applicando AGATHA al programma sul prodotto tra matrici, ho notato risultati diversi secondo il processore su cui hanno girato: sul mio portatile (dual-core) la configurazione migliore è quella con 32 thread su un solo processo, con politica di mapping FALSE mentre su un fisso da 8 core la più veloce ha 2 core e 2 processi. Notevole resta comunque il divario tra soluzioni ottimali e non. Scegliere una configurazione non ideale può aumentare notevolmente i tempi di esecuzione, fino a moltiplicarli per varie volte.

La programmazione parallela ibrida permette di ottenere sistemi a ottime prestazioni. Sfruttando OpenMP si riesce ad ottenere un livello di capillarità ed efficienza irraggiungibile con il solo MPI. D’altro canto MPI fornisce i mezzi per una scalabilità quasi infinita, rendendosi fondamentale e prevalente nel caso di cluster di grande dimensione. Se sfruttate bene, le librerie si completano, poiché ciascuna sopperisce ai difetti dell’altra. Se sfruttate male, finiscono per appesantire e rallentare il problema, causando perdite di efficienza. Fare un programma che, analizzando un’applicazione, consigli dove e come sia possibile rendere parallelo il codice è complesso e va oltre lo scopo di questo progetto. In compenso il programma progettato permette di fare un’analisi sull’implementazione: se i migliori risultati prevedono solo un thread o un processo, allora probabilmente il parallelismo ibrido non è stato fatto correttamente e/o non è efficiente per il problema in questione o anche che i suoi benefici non si sentono sull’architettura su cui deve girare il codice. 