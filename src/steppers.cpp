#include "header.h"
#include <Rcpp.h>
using namespace Rcpp;




void calcMultiplier(const int& geneCount1, const int& geneCount2,
                    double& finalMultiplier,
                    double& geneValue,
                    IntegerMatrix geneInteraction,
                    const int& geneN,
                    double& geneLambda,
                    const double& geneThreshold
){

    double geneActMultiplier=1;

    switch(geneInteraction(geneCount1,geneCount2))
    {
    case 0:
      geneActMultiplier=1.0;
      break;

    case 2:
      geneLambda = 1./geneLambda;
      geneActMultiplier = geneLambda+(1.-geneLambda)*
        1./(1.+std::pow((geneValue/geneThreshold),geneN));
      break;

    case 1:
      geneActMultiplier=(geneLambda+(1.-geneLambda)*
        1./(1.+std::pow((geneValue/geneThreshold),geneN)))/geneLambda;
      break;

    default :
      Rcout << "Invalid Interation code for Gene"<<geneCount1
            <<" and gene"<<geneCount2<<" interaction"<<"\n";
    }
    
    finalMultiplier=finalMultiplier*geneActMultiplier;
}

void stepEM( std::vector <double> &exprxGene,
             std::ofstream &outGE,
             const double &totTime,
             const int &numberGene,
             IntegerMatrix geneInteraction,
             const std::vector<double> &gGene,
             const std::vector<double> &kGene,
             const std::vector<std::vector<int> > &NGene,
             const std::vector<std::vector<double> > &lambda_gene,
             const std::vector<std::vector<double> > &threshold_gene_log,
             const int &possible_interactions,
             const double &standard_deviation_factor,
             const double &D_shot_scaling,
             const std::vector<double> &Darray,
             const int &outputPrecision,
             const double &printStart, const double &printInterval,
             const double &D,
             const double &h){

  double exprxGeneH[numberGene]; //array for temp gene expression values
  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  {
    exprxGeneH[geneCount1] = exprxGene[geneCount1];
  }

  double i=0.0;
  double printTime = printStart;
  do
  {
    i+=h;
    for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
    {
      double finalMultiplier=1;

      for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
      {
        double geneValue=exprxGene[geneCount2];
        double geneThreshold=threshold_gene_log[geneCount1][geneCount2];
        int geneN=NGene[geneCount1][geneCount2];
        double geneLambda=lambda_gene[geneCount1][geneCount2];
        calcMultiplier(geneCount1, geneCount2, finalMultiplier,
                            geneValue, geneInteraction, geneN, geneLambda,
                            geneThreshold);
      }
      exprxGeneH[geneCount1] = exprxGene[geneCount1] +
        h*(gGene[geneCount1]*finalMultiplier-kGene[geneCount1]*
        exprxGene[geneCount1]) +
        D*sqrt(h)*g_distribution(g_generator)*Darray[geneCount1]+
        D_shot_scaling*D*sqrt(h)*g_distribution(g_generator)*
        Darray[geneCount1]*exprxGene[geneCount1];
      if(exprxGeneH[geneCount1]<0) exprxGeneH[geneCount1]=0;
    }

    for(int geneCount1=0;geneCount1<numberGene;geneCount1++){
      exprxGene[geneCount1]=exprxGeneH[geneCount1];}

    if((i> printTime) &&
       (i <= (printTime + printInterval)))
    {
      printTime +=printInterval;
      for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
      {
        outGE<<std::setprecision(outputPrecision)
        <<exprxGene[geneCount1]<<"\t";
      }
      //outGE<<"\n";
    }
  }while(i<totTime);

  // for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  // {
  //   outGE<<std::setprecision(outputPrecision)<<exprxGene[geneCount1]<<"\t";
  // }

}


void stepEM_OU( std::vector <double> &exprxGene,
             std::ofstream &outGE,
             const double &totTime,
             const int &numberGene,
             IntegerMatrix geneInteraction,
             const std::vector<double> &gGene,
             const std::vector<double> &kGene,
             const std::vector<std::vector<int> > &NGene,
             const std::vector<std::vector<double> > &lambda_gene,
             const std::vector<std::vector<double> > &threshold_gene_log,
             const int &possible_interactions,
             const double &standard_deviation_factor,
             const double &D_shot_scaling,
             const std::vector<double> &Darray,
             const int &outputPrecision,
             const double &printStart, const double &printInterval,
             const double &D,
             const double &h,
             const double &ouNoise_t,
             std::vector <double> &prevNoise){

  double exprxGeneH[numberGene]; //array for temp gene expression values
  double currNoise[numberGene]; //array for temp gene expression values
  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  {
    exprxGeneH[geneCount1] = exprxGene[geneCount1];
    prevNoise[geneCount1] = D*Darray[geneCount1] * g_distribution(g_generator);
    
  }

  double i=0.0;
  double printTime = printStart;
  do
  {
    i+=h;
    for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
    {
      double finalMultiplier=1;
      currNoise[geneCount1] = prevNoise[geneCount1] * exp(-h/ouNoise_t) + 
        D*Darray[geneCount1] * sqrt(1-exp(-2*h/ouNoise_t)) * g_distribution(g_generator);

      for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
      {
        double geneValue=exprxGene[geneCount2];
        double geneThreshold=threshold_gene_log[geneCount1][geneCount2];
        int geneN=NGene[geneCount1][geneCount2];
        double geneLambda=lambda_gene[geneCount1][geneCount2];
        calcMultiplier(geneCount1, geneCount2, finalMultiplier,
                            geneValue, geneInteraction, geneN, geneLambda,
                            geneThreshold);
      }

      exprxGeneH[geneCount1] = exprxGene[geneCount1] +
        h*(gGene[geneCount1]*finalMultiplier-kGene[geneCount1]*
        exprxGene[geneCount1]) +
        h*currNoise[geneCount1]+
        D_shot_scaling*D*sqrt(h)*g_distribution(g_generator)*
        Darray[geneCount1]*exprxGene[geneCount1];
      
      if(exprxGeneH[geneCount1]<0) exprxGeneH[geneCount1]=0;
    }

    for(int geneCount1=0;geneCount1<numberGene;geneCount1++){
      exprxGene[geneCount1]=exprxGeneH[geneCount1];
      prevNoise[geneCount1]=currNoise[geneCount1];}

    if((i> printTime) &&
       (i <= (printTime + printInterval)))
    {
      printTime +=printInterval;
      for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
      {
        outGE<<std::setprecision(outputPrecision)
        <<exprxGene[geneCount1]<<"\t";
      }
      //outGE<<"\n";
    }
  }while(i<totTime);

  // for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  // {
  //   outGE<<std::setprecision(outputPrecision)<<exprxGene[geneCount1]<<"\t";
  // }

}





void stepEM_OU_Clamp( std::vector <double> &exprxGene,
                std::ofstream &outGE,
                const double &totTime,
                const int &numberGene,
                IntegerMatrix geneInteraction,
                const std::vector<double> &gGene,
                const std::vector<double> &kGene,
                const std::vector<std::vector<int> > &NGene,
                const std::vector<std::vector<double> > &lambda_gene,
                const std::vector<std::vector<double> > &threshold_gene_log,
                const int &possible_interactions,
                const double &standard_deviation_factor,
                const double &D_shot_scaling,
                const std::vector<double> &Darray,
                const int &outputPrecision,
                const double &printStart, const double &printInterval,
                const double &D,
                const double &h,
                const double &ouNoise_t,
                std::vector <double> &prevNoise,
                std::unordered_map<int, std::vector<double>> &clamps,
                const int &modelNo){
  
  double exprxGeneH[numberGene]; //array for temp gene expression values
  double currNoise[numberGene]; //array for temp gene expression values
  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  {
    auto it = clamps.find(geneCount1);
    if (it != clamps.end()) {
      // If clamped, set to the clamped value
      exprxGeneH[geneCount1] = it->second[modelNo];
    } else {
      // else, use ICs
      exprxGeneH[geneCount1] = exprxGene[geneCount1];
    }
    
    prevNoise[geneCount1] = D*Darray[geneCount1] * g_distribution(g_generator);
    
  }
  
  double i=0.0;
  double printTime = printStart;
  do
  {
    i+=h;
    for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
    {
      double finalMultiplier=1;
      currNoise[geneCount1] = prevNoise[geneCount1] * exp(-h/ouNoise_t) + 
        D*Darray[geneCount1] * sqrt(1-exp(-2*h/ouNoise_t)) * g_distribution(g_generator);
      
      
      auto it = clamps.find(geneCount1);
      if (it != clamps.end()) {
        // If clamped, set to the clamped value
        exprxGeneH[geneCount1] = it->second[modelNo];
      } else {
        // else, compute updates
        for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
        {
          double geneValue=exprxGene[geneCount2];
          double geneThreshold=threshold_gene_log[geneCount1][geneCount2];
          int geneN=NGene[geneCount1][geneCount2];
          double geneLambda=lambda_gene[geneCount1][geneCount2];
          calcMultiplier(geneCount1, geneCount2, finalMultiplier,
                         geneValue, geneInteraction, geneN, geneLambda,
                         geneThreshold);
        }
        
        exprxGeneH[geneCount1] = exprxGene[geneCount1] +
          h*(gGene[geneCount1]*finalMultiplier-kGene[geneCount1]*
          exprxGene[geneCount1]) +
          h*currNoise[geneCount1]+
          D_shot_scaling*D*sqrt(h)*g_distribution(g_generator)*
          Darray[geneCount1]*exprxGene[geneCount1];
        
        if(exprxGeneH[geneCount1]<0) exprxGeneH[geneCount1]=0;
        
        
      }
      
      
      

    }
    
    for(int geneCount1=0;geneCount1<numberGene;geneCount1++){
      exprxGene[geneCount1]=exprxGeneH[geneCount1];
      prevNoise[geneCount1]=currNoise[geneCount1];}
    
    if((i> printTime) &&
       (i <= (printTime + printInterval)))
    {
      printTime +=printInterval;
      for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
      {
        outGE<<std::setprecision(outputPrecision)
             <<exprxGene[geneCount1]<<"\t";
      }
      //outGE<<"\n";
    }
  }while(i<totTime);
  
  // for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  // {
  //   outGE<<std::setprecision(outputPrecision)<<exprxGene[geneCount1]<<"\t";
  // }
  
}



///////////////////////////////////////////////////////////////////////////////

//Runge Kutta fourth order
///////////////////////////////////////////////////////////////////////////////
void stepRK4( std::vector <double> &exprxGene,
             std::ofstream &outGE,
             const double &totTime,
             const int &numberGene,
             IntegerMatrix geneInteraction,
             const std::vector<double> &gGene,
             const std::vector<double> &kGene,
             const std::vector<std::vector<int> > &NGene,
             const std::vector<std::vector<double> > &lambda_gene,
             const std::vector<std::vector<double> > &threshold_gene_log,
             const int &possible_interactions,
             const double &standard_deviation_factor,
             const int &outputPrecision,
             const double &printStart, const double &printInterval,
             const double &h){

double exprxGeneH1[numberGene]; //array for temp gene expression values
double exprxGeneH2[numberGene]; //array for temp gene expression values
double exprxGeneH3[numberGene]; //array for temp gene expression values
double exprxGeneH4[numberGene]; //array for temp gene expression values

for(int geneCountTmp=0;geneCountTmp<numberGene;geneCountTmp++)
{
  exprxGeneH1[geneCountTmp]=exprxGene[geneCountTmp];
  exprxGeneH2[geneCountTmp]=exprxGene[geneCountTmp];
  exprxGeneH3[geneCountTmp]=exprxGene[geneCountTmp];
  exprxGeneH4[geneCountTmp]=exprxGene[geneCountTmp];
}
double i=0.0;
double printTime = printStart;
do
{
  i+=h;
  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  {
    double finalMultiplier=1;

    for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
    {
      double geneValue=exprxGene[geneCount2];
      double geneThreshold=threshold_gene_log[geneCount1][geneCount2];
      int geneN=NGene[geneCount1][geneCount2];
      double geneLambda=lambda_gene[geneCount1][geneCount2];
      calcMultiplier(geneCount1, geneCount2, finalMultiplier,
                     geneValue, geneInteraction, geneN, geneLambda,
                     geneThreshold);
    }


    exprxGeneH1[geneCount1]=h*(gGene[geneCount1]*finalMultiplier -
      kGene[geneCount1]*exprxGene[geneCount1]);
  }

  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  {
    double finalMultiplier=1;

    for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
    {
      double geneValue=exprxGene[geneCount2] +
        0.5*exprxGeneH1[geneCount2];
      double geneThreshold=threshold_gene_log[geneCount1][geneCount2];
      int geneN=NGene[geneCount1][geneCount2];
      double geneLambda=lambda_gene[geneCount1][geneCount2];
      calcMultiplier(geneCount1, geneCount2, finalMultiplier,
                     geneValue, geneInteraction, geneN, geneLambda,
                     geneThreshold);
    }


    exprxGeneH2[geneCount1]=h*((gGene[geneCount1])*
      finalMultiplier-kGene[geneCount1]*(exprxGene[geneCount1] +
      0.5*exprxGeneH1[geneCount1]));
  }

  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  {
    double finalMultiplier=1;

    for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
    {
      double geneValue=exprxGene[geneCount2] +
        0.5*exprxGeneH2[geneCount2];
      double geneThreshold=threshold_gene_log[geneCount1][geneCount2];
      int geneN=NGene[geneCount1][geneCount2];
      double geneLambda=lambda_gene[geneCount1][geneCount2];
      calcMultiplier(geneCount1, geneCount2, finalMultiplier,
                     geneValue, geneInteraction, geneN, geneLambda,
                     geneThreshold);
    }


    exprxGeneH3[geneCount1]=h*((gGene[geneCount1])*finalMultiplier-
      kGene[geneCount1]*(exprxGene[geneCount1] +
      0.5*exprxGeneH2[geneCount1]));
  }


  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  {
    double finalMultiplier=1;

    for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
    {
      double geneValue=exprxGene[geneCount2] +
        exprxGeneH3[geneCount2];
      double geneThreshold=threshold_gene_log[geneCount1][geneCount2];
      int geneN=NGene[geneCount1][geneCount2];
      double geneLambda=lambda_gene[geneCount1][geneCount2];
      calcMultiplier(geneCount1, geneCount2, finalMultiplier,
                     geneValue, geneInteraction, geneN, geneLambda,
                     geneThreshold);
    }


    exprxGeneH4[geneCount1]=h*((gGene[geneCount1])*finalMultiplier-
      kGene[geneCount1]*(exprxGene[geneCount1]+
      exprxGeneH3[geneCount1]));
  }


  /////////////////////////////////////////////////////////////
  for(int geneCount1=0;geneCount1<numberGene;geneCount1++){
    exprxGene[geneCount1] = exprxGene[geneCount1]+
      (exprxGeneH1[geneCount1]+2*exprxGeneH2[geneCount1]+
      2*exprxGeneH3[geneCount1]+exprxGeneH4[geneCount1])/6;
    if(exprxGene[geneCount1]<0) exprxGene[geneCount1]=0;
    }


  if((i> printTime) &&
     (i <= (printTime + printInterval)))
  {
    printTime +=printInterval;
    //std::cout<<i<<"\n";
    for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
    {
      outGE<<std::setprecision(outputPrecision)
            <<exprxGene[geneCount1]<<"\t";
    }
    //outGE<<"\n";
  }
}while(i<totTime);
}



void stepDP( std::vector <double> &exprxGene,
              std::ofstream &outGE,
              const double &totTime,
              const int &numberGene,
              IntegerMatrix geneInteraction,
              const std::vector<double> &gGene,
              const std::vector<double> &kGene,
              const std::vector<std::vector<int> > &NGene,
              const std::vector<std::vector<double> > &lambda_gene,
              const std::vector<std::vector<double> > &threshold_gene_log,
              const int &possible_interactions,
              const double &standard_deviation_factor,
              const int &outputPrecision,
              const double &printStart, const double &printInterval,
              double h, const double &rkTolerance){
  double exprxGeneH[numberGene]; //array for temp gene expression values
  double exprxGeneH1[numberGene]; //array for temp gene expression values
  double exprxGeneH2[numberGene]; //array for temp gene expression values
  double exprxGeneH3[numberGene]; //array for temp gene expression values
  double exprxGeneH4[numberGene]; //array for temp gene expression values
  double exprxGeneH5[numberGene]; //array for temp gene expression values
  double exprxGeneH6[numberGene]; //array for temp gene expression values
  double exprxGeneH7[numberGene]; //array for temp gene expression values

  for(int geneCountTmp=0;geneCountTmp<numberGene;geneCountTmp++)
  {
    exprxGeneH[geneCountTmp]=exprxGene[geneCountTmp];
    exprxGeneH1[geneCountTmp]=exprxGene[geneCountTmp];
    exprxGeneH2[geneCountTmp]=exprxGene[geneCountTmp];
    exprxGeneH3[geneCountTmp]=exprxGene[geneCountTmp];
    exprxGeneH4[geneCountTmp]=exprxGene[geneCountTmp];
    exprxGeneH5[geneCountTmp]=exprxGene[geneCountTmp];
    exprxGeneH6[geneCountTmp]=exprxGene[geneCountTmp];
    exprxGeneH7[geneCountTmp]=exprxGene[geneCountTmp];
  }

  double i=0.0;
  int printCounter = 0;

  do
  {
    for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
    {
      double finalMultiplier=1;

      for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
      {
        double geneValue=exprxGene[geneCount2];
        double geneThreshold=threshold_gene_log[geneCount1][geneCount2];
        int geneN=NGene[geneCount1][geneCount2];
        double geneLambda=lambda_gene[geneCount1][geneCount2];
        calcMultiplier(geneCount1, geneCount2, finalMultiplier,
                       geneValue, geneInteraction, geneN, geneLambda,
                       geneThreshold);
      }


      exprxGeneH1[geneCount1]=h*(gGene[geneCount1]*finalMultiplier-
        kGene[geneCount1]*exprxGene[geneCount1]);
    }

    for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
    {
      double finalMultiplier=1;

      for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
      {
        double geneValue=exprxGene[geneCount2]+
          0.2*exprxGeneH1[geneCount2];
        double geneThreshold=threshold_gene_log[geneCount1][geneCount2];
        int geneN=NGene[geneCount1][geneCount2];
        double geneLambda=lambda_gene[geneCount1][geneCount2];
        calcMultiplier(geneCount1, geneCount2, finalMultiplier,
                       geneValue, geneInteraction, geneN, geneLambda,
                       geneThreshold);
      }


      exprxGeneH2[geneCount1]=h*((gGene[geneCount1])*finalMultiplier-
        kGene[geneCount1]*(exprxGene[geneCount1] +
        0.2*exprxGeneH1[geneCount1]));
    }

    for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
    {
      double finalMultiplier=1;

      for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
      {
        double geneValue=exprxGene[geneCount2]+
          (0.25*exprxGeneH1[geneCount2]+
          0.75*exprxGeneH2[geneCount2])*0.3;
        double geneThreshold=threshold_gene_log[geneCount1][geneCount2];
        int geneN=NGene[geneCount1][geneCount2];
        double geneLambda=lambda_gene[geneCount1][geneCount2];
        calcMultiplier(geneCount1, geneCount2, finalMultiplier,
                       geneValue, geneInteraction, geneN, geneLambda,
                       geneThreshold);
      }


      exprxGeneH3[geneCount1]=h*((gGene[geneCount1])*finalMultiplier-
        kGene[geneCount1]*(exprxGene[geneCount1]+
        (0.25*exprxGeneH1[geneCount1]+
        0.75*exprxGeneH2[geneCount1])*0.3));
    }


    for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
    {
      double finalMultiplier=1;

      for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
      {
        double geneValue=exprxGene[geneCount2]+
          0.8*(((11./9.)*exprxGeneH1[geneCount2]+
          (-14./3.)*exprxGeneH2[geneCount2])+
          (40./9.)*exprxGeneH3[geneCount2]);
        double geneThreshold=threshold_gene_log[geneCount1][geneCount2];
        int geneN=NGene[geneCount1][geneCount2];
        double geneLambda=lambda_gene[geneCount1][geneCount2];
        calcMultiplier(geneCount1, geneCount2, finalMultiplier,
                       geneValue, geneInteraction, geneN, geneLambda,
                       geneThreshold);
      }


      exprxGeneH4[geneCount1]=h*((gGene[geneCount1])*finalMultiplier-
        kGene[geneCount1]*(exprxGene[geneCount1]+
        0.8*(((11./9.)*exprxGeneH1[geneCount1]+
        (-14./3.)*exprxGeneH2[geneCount1])+
        (40./9.)*exprxGeneH3[geneCount1])));
    }

    for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
    {
      double finalMultiplier=1;

      for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
      {
        double geneValue=exprxGene[geneCount2]+(8./9.)*
          ((4843./1458.)*exprxGeneH1[geneCount2]+
          (-3170./243.)*exprxGeneH2[geneCount2]+
          (8056./729.)*exprxGeneH3[geneCount2]+
          (-53./162.)*exprxGeneH4[geneCount2]);

        double geneThreshold=threshold_gene_log[geneCount1][geneCount2];
        int geneN=NGene[geneCount1][geneCount2];
        double geneLambda=lambda_gene[geneCount1][geneCount2];
        calcMultiplier(geneCount1, geneCount2, finalMultiplier,
                       geneValue, geneInteraction, geneN, geneLambda,
                       geneThreshold);
      }


      exprxGeneH5[geneCount1]=h*((gGene[geneCount1])*finalMultiplier-
        kGene[geneCount1]*(exprxGene[geneCount1]+(8./9.)*
        ((4843./1458.)*exprxGeneH1[geneCount1]+
        (-3170./243.)*exprxGeneH2[geneCount1]+
        (8056./729.)*exprxGeneH3[geneCount1]+
        (-53./162.)*exprxGeneH4[geneCount1])));
    }


    for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
    {
      double finalMultiplier=1;

      for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
      {
        double geneValue=exprxGene[geneCount2]+
          ((9017./3168.)*exprxGeneH1[geneCount2]+
          (-355./33.)*exprxGeneH2[geneCount2]+
          (46732./5247.)*exprxGeneH3[geneCount2]+
          (49./176.)*exprxGeneH4[geneCount2]+
          (-5103./18656.)*exprxGeneH5[geneCount2]);
        double geneThreshold=threshold_gene_log[geneCount1][geneCount2];
        int geneN=NGene[geneCount1][geneCount2];
        double geneLambda=lambda_gene[geneCount1][geneCount2];
        calcMultiplier(geneCount1, geneCount2, finalMultiplier,
                       geneValue, geneInteraction, geneN, geneLambda,
                       geneThreshold);
      }


      exprxGeneH6[geneCount1]=h*((gGene[geneCount1])*finalMultiplier-
        kGene[geneCount1]*(exprxGene[geneCount1]+
        (9017./3168.)*exprxGeneH1[geneCount1]+
        (-355./33.)*exprxGeneH2[geneCount1]+
        (46732./5247.)*exprxGeneH3[geneCount1]+
        (49./176.)*exprxGeneH4[geneCount1]+
        (-5103./18656.)*exprxGeneH5[geneCount1]));
    }



    for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
    {
      double finalMultiplier=1;

      for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
      {
        double geneValue=exprxGene[geneCount2]+
          ((35./384.)*exprxGeneH1[geneCount2]+
          (500./113.)*exprxGeneH3[geneCount2]+
          (125./192.)*exprxGeneH4[geneCount2]+
          (-2187./6784.)*exprxGeneH5[geneCount2]+
          (11./84.)*exprxGeneH6[geneCount2]);
        double geneThreshold=threshold_gene_log[geneCount1][geneCount2];
        int geneN=NGene[geneCount1][geneCount2];
        double geneLambda=lambda_gene[geneCount1][geneCount2];
        calcMultiplier(geneCount1, geneCount2, finalMultiplier,
                       geneValue, geneInteraction, geneN, geneLambda,
                       geneThreshold);
      }


      exprxGeneH7[geneCount1]=h*((gGene[geneCount1])*finalMultiplier-
        kGene[geneCount1]*(exprxGene[geneCount1]+
        (35./384.)*exprxGeneH1[geneCount1]+
        (500./113.)*exprxGeneH3[geneCount1]+
        (125./192.)*exprxGeneH4[geneCount1]+
        (-2187./6784.)*exprxGeneH5[geneCount1]+
        (11./84.)*exprxGeneH6[geneCount1]));
    }
    double max_diff_o4_o5=0;
    /////////////////////////////////////////////////////////////
    for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
    {
      exprxGeneH[geneCount1]=exprxGene[geneCount1]+
        (5179./57600.)*exprxGeneH1[geneCount1]+
        (7571./16695.)*exprxGeneH3[geneCount1]+
        (393./640.)*exprxGeneH4[geneCount1]+
        (-92097./339200.)*exprxGeneH5[geneCount1]+
        (187./2100.)*exprxGeneH6[geneCount1]+
        (1./40.)*exprxGeneH7[geneCount1];
      if(exprxGeneH[geneCount1]<0) exprxGeneH[geneCount1]=0;

      exprxGene[geneCount1]=exprxGene[geneCount1]+
        (35./384.)*exprxGeneH1[geneCount1]+
        (500./1113.)*exprxGeneH3[geneCount1]+
        (125./192.)*exprxGeneH4[geneCount1]+
        (-2187./6784.)*exprxGeneH5[geneCount1]+
        (11./84.)*exprxGeneH6[geneCount1];

      if(exprxGene[geneCount1]<0) exprxGene[geneCount1]=0;

      double diff_o4_o5=exprxGene[geneCount1] -
        exprxGeneH[geneCount1];

      diff_o4_o5 = diff_o4_o5 >= 0 ? diff_o4_o5 : -diff_o4_o5;
      max_diff_o4_o5 = max_diff_o4_o5 > diff_o4_o5 ? max_diff_o4_o5 :
        diff_o4_o5;

    }
    double s_rk = h*rkTolerance/(2*(totTime)*max_diff_o4_o5);

    s_rk=std::pow(s_rk,0.25);
    //Rcout<<s_rk<<"\n";
    if(s_rk<1) {
      if(h>0.00001)
      {h=0.5*h;}
      //Rcout<<"decrease h\n";
    }
    else {
      //Rcout<<"increase h\n";
      i+=h;
      if(s_rk>2) {if(h<0.5) {h=2*h;}}

    }

    for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
    {
      exprxGeneH[geneCount1]=exprxGene[geneCount1];


    }
    if((i> (printStart + printInterval*printCounter)) &&
       i <= (h+printStart + printInterval*printCounter))
    {

      printCounter++;
      //std::cout<<i<<"\n";
      for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
      {
        outGE<<std::setprecision(outputPrecision)
              <<exprxGene[geneCount1]<<"\t";
      }
      //outGE<<"\n";
    }
    //i=i+h;
  }while(i<totTime);

}



// Generate a vector of mpr multipliers to gradually increase, then decrease signal
std::vector<double> calcSigValues(const double &totTime,
                                    const double &h,
                                    const double &maxFC) {

    double nSteps = totTime / h;
    double increasingSteps = ceil(nSteps/4);
    double currentVal;
    double mpr_delta = (maxFC-1) / increasingSteps;
    std::vector<double> values;


    // add increasing FC values up to maxFC
    for(int i = 0; i<increasingSteps; i++) {
      currentVal = 1 + (i * mpr_delta);
      values.push_back(currentVal);
    }

    // hold signal for the same duration here
    for(int i = 0; i< increasingSteps; i++) {
      values.push_back(maxFC);
    }

    // decreasing signal
    for(int i = 0; i<increasingSteps; i++) {
      currentVal = maxFC - (i * mpr_delta);
      values.push_back(currentVal);
    }

    // remaining simulation time at base rate
    for(int i = values.size(); i<nSteps; i++) {
      values.push_back(1);
    }

    return values;
}



void stepEM_sig_grad( std::vector <double> &exprxGene,
             std::ofstream &outGE,
             const double &totTime,
             const int &numberGene,
             IntegerMatrix geneInteraction,
             const std::vector<double> &gGene,
             const std::vector<double> &kGene,
             const std::vector<std::vector<int> > &NGene,
             const std::vector<std::vector<double> > &lambda_gene,
             const std::vector<std::vector<double> > &threshold_gene_log,
             const int &possible_interactions,
             const double &standard_deviation_factor,
             const double &D_shot_scaling,
             const std::vector<double> &Darray,
             const int &outputPrecision,
             const double &printStart, const double &printInterval,
             const double &D,
             const double &h,
             const int &sigGene,
             const double &maxFC){

  double exprxGeneH[numberGene]; //array for temp gene expression values
  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  {
    exprxGeneH[geneCount1] = exprxGene[geneCount1];
  }



  // initialize 2D array of signaling multipliers
  int numSteps = static_cast<int>(totTime/h);
  Rcout << "numsteps = " << numSteps << "\n";
  Rcout << "sigGene = " << sigGene << "\n";
  Rcout << "maxFC = " << maxFC << "\n";

  std::vector<std::vector<double> > signalMultipliers(
    numberGene,
    std::vector<double>(numSteps, 1));
  //Rcout << "created signalMultipliers" << "\n";
  std::vector<double> signalValues = calcSigValues(totTime, h, maxFC);
  //Rcout << "created signalValues: " << "\n";
  //for(int i = 0; i<numSteps; i++) {
  //  Rcout << signalValues[i] << " ";
  //}
  //Rcout << "\nFilling signalMultipliers row " << sigGene << "\n";
  for(int i = 0; i<numSteps; i++) {
    //Rcout << i << " ";
    signalMultipliers[sigGene][i] = signalValues[i];
  }
  /*Rcout << "filled signalMultipliers with values";
  for(int i = 0; i < numberGene; i++) {
		for(int j = 0; j < numSteps; j++) {
			Rcout << signalMultipliers[i][j] << " ";
		}
		Rcout << "\n";
  }*/

  //Rcout << "beginning do while loop";
  double i=0.0;
  int currentStep = 0;
  double printTime = printStart;
  do
  {
    i+=h;
    currentStep++;
    for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
    {
      double finalMultiplier=1;

      for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
      {
        double geneValue=exprxGene[geneCount2];
        double geneThreshold=threshold_gene_log[geneCount1][geneCount2];
        int geneN=NGene[geneCount1][geneCount2];
        double geneLambda=lambda_gene[geneCount1][geneCount2];
        calcMultiplier(geneCount1, geneCount2, finalMultiplier,
                            geneValue, geneInteraction, geneN, geneLambda,
                            geneThreshold);
      }
      exprxGeneH[geneCount1] = exprxGene[geneCount1] +
        h*(gGene[geneCount1]*signalMultipliers[geneCount1][currentStep-1]*finalMultiplier-kGene[geneCount1]*
        exprxGene[geneCount1]) +
        D*sqrt(h)*g_distribution(g_generator)*Darray[geneCount1]+
        D_shot_scaling*D*sqrt(h)*g_distribution(g_generator)*
        Darray[geneCount1]*exprxGene[geneCount1];
      if(exprxGeneH[geneCount1]<0) exprxGeneH[geneCount1]=0;
    }

    for(int geneCount1=0;geneCount1<numberGene;geneCount1++){
      exprxGene[geneCount1]=exprxGeneH[geneCount1];}

    if((i> printTime) &&
       (i <= (printTime + printInterval)))
    {
      printTime +=printInterval;
      for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
      {
        outGE<<std::setprecision(outputPrecision)
        <<exprxGene[geneCount1]<<"\t";
      }
      //outGE<<"\n";
    }
  }while(i<totTime);

  // for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  // {
  //   outGE<<std::setprecision(outputPrecision)<<exprxGene[geneCount1]<<"\t";
  // }

}


//Runge Kutta fourth order
///////////////////////////////////////////////////////////////////////////////
void stepRK4_sig_grad( std::vector <double> &exprxGene,
             std::ofstream &outGE,
             const double &totTime,
             const int &numberGene,
             IntegerMatrix geneInteraction,
             const std::vector<double> &gGene,
             const std::vector<double> &kGene,
             const std::vector<std::vector<int> > &NGene,
             const std::vector<std::vector<double> > &lambda_gene,
             const std::vector<std::vector<double> > &threshold_gene_log,
             const int &possible_interactions,
             const double &standard_deviation_factor,
             const int &outputPrecision,
             const double &printStart, const double &printInterval,
             const double &h,
             const int &sigGene,
             const double &maxFC){



 // initialize 2D array of signaling multipliers
 int numSteps = static_cast<int>(totTime/h);
 Rcout << "numsteps = " << numSteps << "\n";
 Rcout << "sigGene = " << sigGene << "\n";
 Rcout << "maxFC = " << maxFC << "\n";

 std::vector<std::vector<double> > signalMultipliers(
   numberGene,
   std::vector<double>((numSteps+5), 1));

 std::vector<double> signalValues = calcSigValues(totTime, h, maxFC);

 for(int i = 0; i<numSteps; i++) {
   signalMultipliers[sigGene][i] = signalValues[i];
 }

double exprxGeneH1[numberGene]; //array for temp gene expression values
double exprxGeneH2[numberGene]; //array for temp gene expression values
double exprxGeneH3[numberGene]; //array for temp gene expression values
double exprxGeneH4[numberGene]; //array for temp gene expression values

for(int geneCountTmp=0;geneCountTmp<numberGene;geneCountTmp++)
{
  exprxGeneH1[geneCountTmp]=exprxGene[geneCountTmp];
  exprxGeneH2[geneCountTmp]=exprxGene[geneCountTmp];
  exprxGeneH3[geneCountTmp]=exprxGene[geneCountTmp];
  exprxGeneH4[geneCountTmp]=exprxGene[geneCountTmp];
}
double i=0.0;
int currentStep = 0;
double printTime = printStart;
do
{
  i+=h;
  currentStep++;
  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  {
    double finalMultiplier=1;

    for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
    {
      double geneValue=exprxGene[geneCount2];
      double geneThreshold=threshold_gene_log[geneCount1][geneCount2];
      int geneN=NGene[geneCount1][geneCount2];
      double geneLambda=lambda_gene[geneCount1][geneCount2];
      calcMultiplier(geneCount1, geneCount2, finalMultiplier,
                     geneValue, geneInteraction, geneN, geneLambda,
                     geneThreshold);
    }


    exprxGeneH1[geneCount1]=h*(gGene[geneCount1]*signalMultipliers[geneCount1][currentStep-1]*finalMultiplier -
      kGene[geneCount1]*exprxGene[geneCount1]);
  }

  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  {
    double finalMultiplier=1;

    for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
    {
      double geneValue=exprxGene[geneCount2] +
        0.5*exprxGeneH1[geneCount2];
      double geneThreshold=threshold_gene_log[geneCount1][geneCount2];
      int geneN=NGene[geneCount1][geneCount2];
      double geneLambda=lambda_gene[geneCount1][geneCount2];
      calcMultiplier(geneCount1, geneCount2, finalMultiplier,
                     geneValue, geneInteraction, geneN, geneLambda,
                     geneThreshold);
    }


    double newMultiplier = (signalMultipliers[geneCount1][currentStep-1] + signalMultipliers[geneCount1][currentStep]) / 2;
    exprxGeneH2[geneCount1]=h*((gGene[geneCount1])*newMultiplier*
      finalMultiplier-kGene[geneCount1]*(exprxGene[geneCount1] +
      0.5*exprxGeneH1[geneCount1]));
  }

  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  {
    double finalMultiplier=1;

    for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
    {
      double geneValue=exprxGene[geneCount2] +
        0.5*exprxGeneH2[geneCount2];
      double geneThreshold=threshold_gene_log[geneCount1][geneCount2];
      int geneN=NGene[geneCount1][geneCount2];
      double geneLambda=lambda_gene[geneCount1][geneCount2];
      calcMultiplier(geneCount1, geneCount2, finalMultiplier,
                     geneValue, geneInteraction, geneN, geneLambda,
                     geneThreshold);
    }

    double newMultiplier = (signalMultipliers[geneCount1][currentStep-1] + signalMultipliers[geneCount1][currentStep]) / 2;
    exprxGeneH3[geneCount1]=h*((gGene[geneCount1])*newMultiplier*finalMultiplier-
      kGene[geneCount1]*(exprxGene[geneCount1] +
      0.5*exprxGeneH2[geneCount1]));
  }


  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  {
    double finalMultiplier=1;

    for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
    {
      double geneValue=exprxGene[geneCount2] +
        exprxGeneH3[geneCount2];
      double geneThreshold=threshold_gene_log[geneCount1][geneCount2];
      int geneN=NGene[geneCount1][geneCount2];
      double geneLambda=lambda_gene[geneCount1][geneCount2];
      calcMultiplier(geneCount1, geneCount2, finalMultiplier,
                     geneValue, geneInteraction, geneN, geneLambda,
                     geneThreshold);
    }


    exprxGeneH4[geneCount1]=h*((gGene[geneCount1])*signalMultipliers[geneCount1][currentStep]*finalMultiplier-
      kGene[geneCount1]*(exprxGene[geneCount1]+
      exprxGeneH3[geneCount1]));
  }


  /////////////////////////////////////////////////////////////
  for(int geneCount1=0;geneCount1<numberGene;geneCount1++){
    exprxGene[geneCount1] = exprxGene[geneCount1]+
      (exprxGeneH1[geneCount1]+2*exprxGeneH2[geneCount1]+
      2*exprxGeneH3[geneCount1]+exprxGeneH4[geneCount1])/6;
    if(exprxGene[geneCount1]<0) exprxGene[geneCount1]=0;
    }


  if((i> printTime) &&
     (i <= (printTime + printInterval)))
  {
    printTime +=printInterval;
    //std::cout<<i<<"\n";
    for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
    {
      outGE<<std::setprecision(outputPrecision)
            <<exprxGene[geneCount1]<<"\t";
    }
    //outGE<<"\n";
  }
}while(i<totTime);
}

