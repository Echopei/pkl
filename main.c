#include<stdio.h>
#include<getopt.h>

typedef struct {

	char **input_bams ;
	int  n ,m ;//malloc
}  input_bams_t ;

/*  */

//init()
// push  
// free()

typedef  struct {

	input_bams_t inputfile ;
	int	min_baseq ; //   base quality >= 
	char  *region_string ;
	char  *outfile ;

} cmd_t ;

// init 
//

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
    struct option longopts[]={
      {"in",required_argument,NULL,'i'},
      {"out",required_argument,NULL,'o'},
      {"region",required_argument,NULL,'r'},
      {"Support",required_argument,NULL,'S'},
      {"mode",required_argument,NULL,'s'},
      {"mapping quality",required_argument,NULL,'m'},
      {"base quality",required_argument,NULL,'b'},
      {"proper_pair",required_argument,NULL,'p'},
      {0,0,0,0} 
    };
    if(argc < 2) return RegionParalle_usage();
    else{
      while((opt = getopt_long(argc,argv,"i:o:r:S:s:m:b:p:",longopts,NULL))!=-1){
        switch(opt){
          case 'i':
          case 'o': fprintf(stderr,"%s\n",optarg);break;
          case 'r': fprintf(stderr,"%s\n",optarg);break;
          case 'S':
          case 's':
          case 'm':
          case 'b': p.min_baseq =  atoi(optarg); break ;
          case 'p': fprintf(stderr,"%s\n",optarg);break;
          case '?': fprintf(stderr,"unknown option: %s\n",optopt);break;
          default: return 1;
       }  
     }
    }
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
