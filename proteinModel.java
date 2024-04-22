import java.util.Scanner;  // Import the Scanner class

public class proteinModel {
	//entrada: vetor estados iniciais das proteínas,fatores e fenótipos em t = 0; número de iterações k que serão simuladas
	//saída:n vetores de k elementos cada, onde cada vetor é o estado de uma proteína/fator/fenótipo ao longo de k iterações
	//ERK[i] estado da proteína ERK na iteração i

	//Objeto das proteínas
	private static class Protein {
		public boolean state;
		public int number;
		public int[] regulator; //números das proteínas reguladores de protein

		public Protein(boolean state, int number,int[] regulator) {
			this.state=state;
			this.number=number;
			this.regulator=regulator;
		}

		public Protein(int number,int[] regulator) {
			this.number=number;
			this.regulator=regulator;
		}

		public Protein(int number,Stack<Integer> regulatorStack) {
			this.number=number;
			int[] regulatorArray=new int[regulatorStack.size()];
			for (int i =0;i<regulatorStack.size() ; i++) {
				regulatorArray[i]=regulatorStack.pop();
			}
			this.regulator=regulatorArray;
		}

		public Protein setState(int s) {
			if (s==0) this.state=false;
			else this.state=true;
			return this;
		}
	} 

	//Implementando grafo direcionado
    public static class Digraph {
		private final int V;							// número de vértices do grafo
		private int E;									// número de arestas do grafo
		public ST<Integer, Integer>[] adj;				// adj[v] = lista de adjacência para o vértice v
		private int K;
		public int dist;

		public Digraph(In in) {  

    		String line1 = in.readLine();

    		String[] nmk = line1.split(" ");
 
        	V = Integer.parseInt(nmk[0]);
        	E = Integer.parseInt(nmk[1]);
        	K = Integer.parseInt(nmk[2]);

        	adj = (ST<Integer,Integer>[]) new ST[V+1];
        	for (int v = 1; v < V+1; v++) {
                adj[v] = new ST<Integer, Integer>();
            }

        	int i =0;
            while(i<E) {
        		String line = in.readLine();
        		String[] abt = line.split(" ");
        		int a = Integer.parseInt(abt[0]);
        		int b = Integer.parseInt(abt[1]);
        		int t = Integer.parseInt(abt[2]);
        		addEdge(a,b,t); 
        		i++;
        	}

        }
		
		public int V() { return V; }
		
		public int E() { return E; }

		public int K() { return K; }
		
		public void addEdge(int a, int b, int t) {
			adj[a].put(b,t);							//p: 0  ou 1: 1 ativação, 0 inibição
		}
		
		public Iterable<Integer> adj(int v) { 
			return adj[v]; 
		}

		public String toString() {
			return(V + " vertices, " + E + " arestas" );
		}
    }

    public static Stack<Integer> which_regulators(Digraph g,int cur) {   //cur= número da proteína da
			Stack<Integer> regulators = new Stack<Integer>();
			
			for(int i=1;i<g.V()+1;i++) {
				if(g.adj[i].contains(cur)==true) {
					regulators.push(i);
				}
			}
			return regulators;
		}

	public static boolean is_Active(Digraph g, Protein p, Protein[] prots) {		
		Stack<Integer> thisRegs = which_regulators(g,p.number);
		int l = thisRegs.size();

		int ativ = 0;
		int inib = 0;
		for(int i=0;i<l;i++) {
			int regulator = thisRegs.pop();
			if (/*is_Active(g,prots[regulator],prots)==true||*/prots[regulator].state==true)	{
				if (g.adj[regulator].get(p.number)==1) ativ++;
				else inib ++;
			}
		}
		if (ativ>inib) return true;
		else if (ativ<inib) return false;
		else return p.state;
	}

	public static Protein[] create_prot(Digraph g, In in){
		Protein[] proteins = new Protein[g.V()+1];

		for(int i =1;i<g.V()+1;i++) {
			Stack<Integer> regulators=which_regulators(g,i);
			proteins[i] = new Protein(i,regulators);
		}

		int i =0;
        while(i<g.K()) {
        	String line = in.readLine();
        	String[] p = line.split(" ");
        	int a = Integer.parseInt(p[0]);
        	int b = Integer.parseInt(p[1]);
        	if(b==0||b==1) proteins[a]=proteins[a].setState(b); 
        	i++;
        }
        return proteins;
	}

	/*public static int levels(int w){
		int[][] levels =  new int[26][5];
		levels[1]={0};
		levels[2]={0};
		levels[3]={0};
		levels[4]={1};
		levels[5]={2,3};
		levels[6]={1};
		levels[7]={1};
		levels[8]={2};
		levels[9]={3,4};
		levels[10]={};
		levels[11]={};
		levels[12]={};
		levels[13]={};
		levels[14]={};
		levels[15]={};
		levels[16]={};
		levels[17]={};
		levels[18]={};
		levels[19]={};
		levels[20]={};
		levels[21]={};
		levels[22]={};
		levels[23]={};
		levels[24]={};
		levels[25]={};

		return levels[w];
	}*/

	public static Protein[] activate(Digraph g, Protein prot, Protein[] prots, int iteration, int[] states) {
		//int p = prot.number;
		//Stack<Integer> thisRegs = which_regulators(g,p);
		//int l = thisRegs.size();
		int cad = states[1];
		int rtk = states[2];
        int itg = states[3];

        if (iteration=0) {
        	//g.adj[cad] usar is active em cada um pra saber se ta active ou nao
        }

        //for iteration 0: cad, rtk and itg act on next proteins
        //any iteration: each node that was just activated/inactivated acts on next nodes.


		//iteration=numero de iterações. a cada iteração, vemos se há algum nó a ser ativado no próximo nível do grafo
        for (int i=0;i<iteration ;i++ ) {
        	for (int j =1;j<prots.length;j++){
        		for(int k=0;k<which_regulators(g,j).size();k++) {
        			Stack<Integer> thisRegs = which_regulators(g,j);
        			if(prots[thisRegs.pop()].state==true) prots[j].setState(1);   				
        		}
        	}
        }
        return prots;
	}

	public static int is_Active_now(Digraph g, Protein prot, Protein[] prots, int iteration, int[] states) {		
        prots = activate(g,prots,iteration,states);
        if (!prots[p].state) return 0;
        else return 1;
        
		/*Stack<Integer> thisRegs = which_regulators(g,p.number);
		int l = thisRegs.size();

		int ativ = 0;
		int inib = 0;
		for(int i=0;i<l;i++) {
			int regulator = thisRegs.pop(); */
		//	if (/*is_Active(g,prots[regulator],prots)==true||*/prots[regulator].state==true)	{
		/*		if (g.adj[regulator].get(p.number)==1) ativ++;
				else inib ++;
			}
		}
		if (ativ>inib) return true;
		else if (ativ<inib) return false;
		else return p.state;*/
		
		//return states[p];
	}

	public static int[][] genSetup(In in, Digraph g) {
		String line1 = in.readLine();
		String[] s = line1.split(" ");

    	int[] states = new int[s.length+1];
   	
   		for (int i =0;i<s.length;i++) {
   	  		states[i+1]=Integer.parseInt(s[i]);
   		}  

   		String line2 = in.readLine();
    	int k = Integer.parseInt(line2);

    	int[][] arr = new int[states.length][k];
    	Protein[] proteins = new Protein[states.length];			//vetor de proteínas

		for(int i =1;i<states.length;i++) {							//completando o vetor 
			Stack<Integer> regulators=which_regulators(g,i);		//vendo os reguladores de cada proteína
			proteins[i] = new Protein(i,regulators);				//criando um objeto para cada proteína
			proteins[i].setState(states[i]);
		}

    	for (int i=1; i<states.length;i++) {
    		for (int j =0;j<k;j++) {			//estado da proteína i na iteração j
    			arr[i][j]=is_Active_now(g,proteins[i],proteins,j,states);
    		}
    	}
    	return arr;
	} 

	public static void main(String[] args) {
		//Criando o grafo
		In in_0 = new In(args[0]);
		Digraph g = new Digraph(in_0);

		Scanner myObj = new Scanner(System.in);  // Create a Scanner object
		System.out.println("General mode (y/n)?");
		String mode = myObj.nextLine();  // Read user input

		if (mode.equals("n")) {
			//Modo básico
			//Proteína que quero saber o estado
			int n = 18;							//é pra dar true

			//Criando objetos
			Protein[] prots = create_prot(g,in_0);
			Protein p = prots[n];	
		
			System.out.print("Estado 18:");
			System.out.println(is_Active(g,p,prots));
			System.out.print("Estado 14:");
			System.out.println(prots[14].state);
			System.out.print("Estado 19:");
			System.out.println(prots[19].state);
		}

		else {
			//Modo geral
			In in = new In(args[1]);
			int [][] gen = genSetup(in, g);
		
    		for (int i =1;i<gen.length;i++ ) {
    			System.out.print("Fator, fenotipo ou proteina " + i + ":           ");
    			for (int j =0;j<gen[i].length ;j++ ) {
    				//System.out.print("      Estado na iteracao " + j + ": " + gen[i][j]);	
    				System.out.print(gen[i][j] + " ");
    			}
    			System.out.println();
    		}

		}
	}
}