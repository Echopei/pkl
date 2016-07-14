#include<stdio.h>
#include<getopt.h>
#include<stdlib.h>
#define DEFAULT_SIZE 1

typedef struct {
	char **input_bams ;
        int n,m;
} input_bams_t ;



int init_input_bams_t(input_bams_t* inputfile)
{
   inputfile = malloc(sizeof(input_bams_t));
   inputfile->m = DEFAULT_SIZE;
   inputfile->n = 0;
   inputfile->input_bams = (char**)malloc(inputfile->m*sizeof(char*));
   return 0;
}
int push_input_bams_t(char *optarg,input_bams_t* inputfile)
{
   if(inputfile->n == inputfile->m){
      inputfile->m = inputfile->m << 1;
      inputfile->input_bams = (char**)realloc(inputfile->input_bams,inputfile->m*sizeof(char*));
   }
   inputfile->input_bams[inputfile->n] = optarg;
   inputfile->n++;
   return 0;
}
void free_input_bams_t(input_bams_t *inputfile)
{
    free(inputfile->input_bams);
}

typedef  struct {
	int	min_baseq ; //   base quality >= 
	char  *region_string ;
	char  *outfile ;
        input_bams_t* inputfile;
} cmd_t ;

void init_cmd_t(int optopt,char* optarg,cmd_t cm)
{
    if(optarg == NULL){
       switch(optopt){
         case 'S': cm.min_baseq = 4;break;
         case 's':
         case 'm':
         case 'p': cm.min_baseq = 0;break;
         case 'b': cm.min_baseq = 2;break;
       }  
    }else{
       switch(optopt){
         case 'i': push_input_bams_t(optarg,cm.inputfile);break;
         case 'o': cm.outfile = optarg;break;
         case 'r': cm.region_string = optarg;break;
         case 'S':
         case 's':
         case 'm':
         case 'p':
         case 'b': cm.min_baseq = atoi(optarg);break;
       }
    }
} 


int RegionParalle_usage()
{
    fprintf(stderr,"Program: gvc -sv RegionParalle\n");
    fprintf(stderr,"Usage:   gvc -sv RegionParalle [-in<filename>-in<filename...] [-out]<filename>][other options]\n");
    fprintf(stderr,"Options:\n");
    fprintf(stderr,"Input&Output:\n");
    fprintf(stderr,"        --in     <Input BAM filename> The input BAM file(s) [stdin]\n");
    fprintf(stderr,"        --out    <Output dot filename> The pup format output of gvc -sv [stdout]\n");
    fprintf(stderr,"        --region <Region> Genomic region.Index file is recommended for better performance, and is read automatically if it exists as <filename>.bai\n");
    fprintf(stderr,"               chr1   -only alignments on (entire) reference 'chr1'\n");
    fprintf(stderr,"               chr1:500 -only alignments overlappingt the region starting at chr1:500 and continuing to the end of chr1\n");
    fprintf(stderr,"              chr1:500-1000 -only alignments overlapping the region starting at chr1:500 and continuing to chr1:1000\n");
    fprintf(stderr,"Filters:\n");
    fprintf(stderr,"        -S INT:   tumor Support num [default : 4]\n");
    fprintf(stderr,"        -s INT 0: high_pre mode 1:high_sen mode [default:0]\n");
    fprintf(stderr,"        -m INT:   mapping quality >= [default:0]\n");
    fprintf(stderr,"        -b INT:   base quality >= [default:2]\n");
    fprintf(stderr,"        -p INT:   proper_pair{0,1} [default:0]\n");
    fprintf(stderr,"        -?:       longer help\n");
    return 1; 
}
int RegionParalle(int argc, char* argv[])
{
    int opt;
    cmd_t p ;
    init_input_bams_t(p.inputfile);
    struct option longopts[]={
      {"in",required_argument,NULL,'i'},
      {"out",required_argument,NULL,'o'},
      {"region",required_argument,NULL,'r'},
      {"Support",optional_argument,NULL,'S'},
      {"mode",optional_argument,NULL,'s'},
      {"mapping quality",optional_argument,NULL,'m'},
      {"base quality",optional_argument,NULL,'b'},
      {"proper_pair",optional_argument,NULL,'p'},
      {0,0,0,0} 
    };
    if(argc < 2) return RegionParalle_usage();
    else{
      while((opt = getopt_long(argc,argv,"i:o:r:S::s::m::b::p::",longopts,NULL))!=-1){
        switch(opt){
          case 'i': 
          case 'o': 
          case 'r': 
          case 'S':
          case 's':
          case 'm': 
          case 'b': 
          case 'p': init_cmd_t(optopt,optarg,p);fprintf(stderr,"%s\n",optarg);break;
          case '?': fprintf(stderr,"unknown option: %s\n",optopt);break;
          default: return 1;
       }  
     }
    }
    free_input_bams_t(p.inputfile);
    return 0;
}

static int usage()
{
    fprintf(stderr, "\n");
    fprintf(stderr,"Program: gvc -sv\n");
    fprintf(stderr,"Version: 1.0\n");
    fprintf(stderr,"Usage:   gvc -sv <command> [options]\n");
    fprintf(stderr,"Command: RegionParalle Paralle according region division\n");
    fprintf(stderr,"         Dot2SV         generate SV type frome Dot FILE\n");
    return 1;

}

int main(int argc, char* argv[])
{
    int ret;
    if(argc < 2) return usage();
    if(strcmp(argv[1], "RegionParalle") == 0) ret = RegionParalle(argc-1,argv+1);
    else{
      fprintf(stderr, "[main] unrecognized command '%s'\n",argv[1]);
      return 1;
    }
    return ret;
}
