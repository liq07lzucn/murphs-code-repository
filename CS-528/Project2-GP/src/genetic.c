/**
 * @file genetic.c
 * @brief Genetic Opperations
 */
#include "genetic.h"

#define MAXPOP 1000


/**
 * @brief counts the number of times the tree occurs
 *      in the forest.
 * @param forest    - the forest of trees
 * @param n         - the tree in question
 * @param numTrees  - number of trees in the forest
 * @return number of times the tree is in the forest,
 *      or 0 if it is unique
 */
int uniqueTree(node **forest, const node *n, int numTrees){
    int unique = 0;
    int tree;
    for (tree = 0; tree < numTrees; tree++){
        if (isequal(n,forest[tree])==1){
            unique ++;
        }
    }
    return unique;
}
/**
 * @brief Creates a forest of trees
 * @param numTrees  - number of trees in the forest
 * @param geneticParam - tree parameters
 */
void createForest(node *forest[],int numTrees, struct geneticParam *param){
    int tree;
    for(tree = 0; tree < numTrees; tree++){
        forest[tree] = buildTree(NULL,param->maxDepth,param->pruneFraction,param->constProb);
    }
}

void rampedHalfHalf(node *forest[], int numTrees, struct geneticParam *param){

    int tree;
    int depth = 0;
    /* Full Trees */
    for(tree = 0; tree < numTrees/2; tree++){
        forest[tree] = buildTree(NULL,param->maxDepth,param->pruneFraction,param->constProb);
    }
    /* Ramped Depths */
    for( ;tree < numTrees; tree++){
        for (depth = 2; depth < param->maxDepth; depth++){
            forest[tree] = buildTree(NULL,param->maxDepth,param->pruneFraction,param->constProb);
        }
    }
}
node* bestTreeSummary(FILE* out,char *filename, double val[][2]){

    char postfixExpr[512];
    double e = 0;
    double tVal;
    int i;
    node *bestTree = NULL;
    FILE *bestTreeIn = fopen(filename,"r");
    if( bestTreeIn){
        fgets(postfixExpr,512,bestTreeIn);
        bestTree = readPostfix(postfixExpr);
        fclose(bestTreeIn);
    }
    if (!bestTree){
        fprintf(stdout,"Could not open or convert %s to a tree\n",filename);
        exit(EXIT_FAILURE);
    }
    fprintf(out,"\nBestTree:\n");
    writePostfixHelper(bestTree,out);
    fprintf(out,"\n");
    fprintf(out," (x,y)  Tree(x)  |y-Tree(x)|\n");
    for (i = 0; i < NUMPOINTS; i++){
        tVal = evalTree(bestTree,val[i][0]);
        e += pow(abs(val[i][1]-tVal),2);
        fprintf(out,"% 5.3f \t % 5.3f % 5.3f % 5.3f\n",val[i][0],val[i][1],tVal,fabs(val[i][1]-tVal));
    }
    fprintf(out,"SSE: % 5.3e\n",e);
    return bestTree;
}
/**
 * @brief deletes a forest of trees
 * @param forest    - the forest of trees
 * @param numTrees  - number of trees in the forest
 */
void deleteForest(node *forest[], int numTrees){
    int tree;
    for( tree = 0; tree < numTrees; tree++){
        deleteTree(forest[tree]);
    }
}
void importData(char *filename, double val[][2]){
    int lineNum = 0;
    FILE *f = fopen(filename,"r");
    char line[128];
    char *p;
    if (f){
        while( fgets(line,sizeof line,f) != NULL && lineNum < NUMPOINTS){
            val[lineNum][0] = strtod(line,&p);
            val[lineNum][1] = strtod(p,NULL);
            lineNum ++;
        }
    }
    else
        fprintf(stderr,"Could not open %s for data import\n",filename);
}
/**
 * @brief Computes the precentage of the population that is unique
 * @param forest    - the forest of trees
 * @param numTrees  - the number of trees in the population
 */
double diversity(node *forest[], int numTrees){
    int t1;
    int t2;
    int numSame = 0;
    int eq[MAXPOP];
    for (t1 = 0; t1 < MAXPOP; t1 ++)
        eq[t1] = 0;
    for( t1 = 0; t1 < numTrees-1; t1++){
        for (t2 = t1+1; t2 < numTrees; t2++)
            if (isequal(forest[t2],forest[t1])){
                eq[t1] = 1; 
                eq[t2] = 1;
            }
    }
    for (t1 = 0; t1 < numTrees; t1++){
        if (eq[t1] == 0)
            numSame ++;
    }

    return 1.0 - ((double) numSame / (double) numTrees);
}
/**
 * @brief Computes the sse
 * @param tree
 * @param 2D array of values
 * @return the Sum Squared Error
 */
double sse(const node *t,  double val[][2]){
    double e = 0;
    double tVal;
    int i;
    for (i = 0; i < NUMPOINTS; i++){
        tVal = evalTree(t,val[i][0]);
        e += pow(val[i][1]-tVal,2);
    }
    return e;
}
/**
 * @brief Computes the SSE of the forest
 * @param forest of trees
 * @param number of trees in forest
 * @param data set
 * @param data sutructre for the generational SSSE, where:
 *          e[0] = worst
 *          e[1] = mean
 *          e[2] = best
 * @param array to hold the sse per index
 * @param bestTreeName - name of the best tree
 * @return the best SSE
 */
double SSE(node *forest[], int numTrees, double val[][2],
        double *e,double *sseError, char *bestTreeName){
    int tree;
    char buffer[128];
    e[0] = 0;           /* Worst (max) */
    e[1] = 0;           /* Mean  */
    e[2] = DBL_MAX;     /* Best (min) */
    for( tree = 0; tree < numTrees; tree++){

        sseError[tree] = sse(forest[tree],val);
        e[1] += sseError[tree];
        if ( sseError[tree] < e[2]){
            e[2] = sseError[tree];
            sprintf(buffer,"%s.dot",bestTreeName);
            writeTree(forest[tree],buffer);
            sprintf(buffer,"%s.postfix",bestTreeName);
            writePostfix(forest[tree],buffer);
        }
        else if ( sseError[tree] > e[0])
            e[0] = sseError[tree];
    }
    /* Computing average */
    e[1] = e[1] / (double) (numTrees);
    return e[2];
}
void mutatePop(node *forest[], int numTrees,double mR){
    int tree = 0;
    for (tree = 0; tree < numTrees; tree++)
        mutate(forest[tree],mR);
}
void crossOver(node *forest[], int numTrees, double sR){
    int tree = 0;
    int t1;
    int t2;
    for (tree = 0; tree < numTrees; tree++){
        t1 = rand() % numTrees;
        t2 = rand() % numTrees;
        swap(forest[t1],forest[t2],sR);
    }
}

int compareSSEPoint(const void *a, const void *b){
    const struct ssePoint *a1 = (struct ssePoint *) a;
    const struct ssePoint *b1 = (struct ssePoint *) b;

    if (a1->sse > b1->sse) 
        return 1;
    else if (a1->sse < b1->sse) 
        return -1;
    else 
        return 0;
}
void breedGeneration(node *forest[], int numTrees, double sseError[], struct geneticParam *param){

    int t1;
    int t2;
    int i;
    struct ssePoint rankSSE[MAXPOP];
    node *newforest[MAXPOP];
    node* temp;
    int tree = 0;
    int spartanTrees = floor(numTrees*param->spartanFraction);
    int tournamentTrees = floor(numTrees* param->touramentFraction);
    int rankTrees = floor(numTrees* param->rankFraction);

    /* Check that all of the population contigents match */
    if (spartanTrees +  tournamentTrees + rankTrees != numTrees){
        fprintf(stderr,"Total number of population constitunts are not the total population\n");
    }

    /* Ranking the trees */
    for (i = 0; i < numTrees; i++){
        rankSSE[i].sse = sseError[i];
        rankSSE[i].index = i;
    }
    qsort(rankSSE,numTrees,sizeof(struct ssePoint),compareSSEPoint);

    for (tree = 0; tree < (rankTrees+spartanTrees); tree++){
        newforest[numTrees-tree-1] = copy(forest[rankSSE[tree].index]);
    }

    /* Tournamnet Selection */
    for (tree=0; tree < tournamentTrees; tree++){
        t1 = rand() % (numTrees);
        t2 = rand() % (numTrees);
        if (sseError[t1] >= sseError[t2])
            /* Keep t2 */
            newforest[tree] = copy(forest[t2]);
        else
            /* Keep t1 */
            newforest[tree] = copy(forest[t1]);
    }

    /* Copying over the trees */
    for (tree = 0; tree < numTrees; tree ++){
        temp = forest[tree];
        forest[tree] = newforest[tree];
        newforest[tree] = temp;
    }
    deleteForest(newforest,numTrees);

    /**
     * Mutation and cross over on the new generation
     * Spartans and fresh trees are excluded from muation.
     */
    mutatePop(forest,(numTrees-spartanTrees),param->mutationRate);
    crossOver(forest,(numTrees-spartanTrees),param->swapRate); 
}
