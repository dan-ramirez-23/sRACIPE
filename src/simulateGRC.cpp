#include "header.h"
#include <Rcpp.h>
#include <utility>

// [[Rcpp::plugins("cpp11")]]



using namespace Rcpp;

void writeParameters(const int &numberGene, const int &outputPrecision,
                     const std::vector<double> &gGene,
                      const std::vector<double> &kGene,
                      const std::vector<std::vector<int> > &nGene,
                      const std::vector<std::vector<double> > &lambdaGene,
                      const std::vector<std::vector<double> > &threshGeneLog,
                      std::ofstream &outParam)
{
  // outParam<<"Parameters"<<"\n";
  /////////////////////////////////////////////////////////////////////////////

  //Writing parameters to file
  /////////////////////////////////////////////////////////////////////////////
  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  {outParam<<std::setprecision(outputPrecision)<<gGene[geneCount1]<<"\t";}
  //production rate of each gene

  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  {outParam<<std::setprecision(outputPrecision)<<kGene[geneCount1]<<"\t";}
  // degradation rate of each gene

  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
    for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
  {if(threshGeneLog[geneCount1][geneCount2]>0)
    outParam<<std::setprecision(outputPrecision)
    <<threshGeneLog[geneCount1][geneCount2]<<"\t";}
  // above--thresholds for the inteaction links, thresholds for
  // inward links for genes are written, starting from gene 1
  // (thresholds for all inward links of gene 1 and so on)
  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
    for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
  {if(nGene[geneCount1][geneCount2]>0)
    outParam<<std::setprecision(1)<<nGene[geneCount1][geneCount2]<<"\t";}
  // above--n for the inteaction links,
  // n for inward links for genes are written, starting from gene 1
  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
    for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
  {if(lambdaGene[geneCount1][geneCount2]>0)
    outParam<<std::setprecision(outputPrecision)
    <<lambdaGene[geneCount1][geneCount2]<<"\t";}
  // above--lambda for the inteaction links, lambda for
  // inward links for genes are written, starting from gene 1
  outParam<<"\n";

}

void readParameters(IntegerMatrix geneInteraction, const int &numberGene,
                    std::vector<double> &gGene,
                     std::vector<double> &kGene,
                     std::vector<std::vector<int> > &nGene,
                     std::vector<std::vector<double> > &lambdaGene,
                     std::vector<std::vector<double> > &threshGeneLog,
                     std::ifstream &inParams)
{
  /////////////////////////////////////////////////////////////////////////////

  //Reading parameters from file
  /////////////////////////////////////////////////////////////////////////////
  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  {    inParams >>  gGene[geneCount1];}

  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  { inParams >>  kGene[geneCount1]; }

  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  {
    for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
    {
      if(geneInteraction(geneCount1,geneCount2)!=0)
      {
        inParams >>  threshGeneLog[geneCount1][geneCount2];
        //Rcout<<"Here"<< threshGeneLog[geneCount1][geneCount2]<<"\n";
      }
    }
  }

  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
    for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
  {double test;
    if(geneInteraction(geneCount1,geneCount2)!=0) {
      inParams >>  test;
      nGene[geneCount1][geneCount2] = std::round(test);}}

  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
    for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
  {if(geneInteraction(geneCount1,geneCount2)!=0)
    inParams >>  lambdaGene[geneCount1][geneCount2];}

}


void selectParameters(Rcpp::IntegerMatrix geneInteraction,
                      Rcpp::NumericVector thresholdGene,
                      const double gMin, const double gMax,
                      const double kMin, const double kMax,
                      const int interactionTypes,
                      const double lambdaMin,
                       const double lambdaMax, const int nMin, const int nMax,
                       const double sdFactor, int numberGene,
                       std::vector<double> &gGene,
                       std::vector<double> &kGene,
                       std::vector<std::vector<int> > &nGene,
                       std::vector<std::vector<double> > &lambdaGene,
                       std::vector<std::vector<double> > &threshGeneLog
                       )
{
  for(int geneCount1=0;geneCount1<numberGene;geneCount1++){
    gGene[geneCount1]=gMin+(gMax-gMin)*u_distribution(u_generator);
    }

  for(int geneCount1=0;geneCount1<numberGene;geneCount1++){
    kGene[geneCount1]=kMin+(kMax-kMin)*u_distribution(u_generator);
    }

  for(int geneCount1=0;geneCount1<numberGene;geneCount1++){
    for(int geneCount2=0;geneCount2<numberGene;geneCount2++){
      if(geneInteraction(geneCount1,geneCount2)==0){
        nGene[geneCount1][geneCount2]=0;
      } else {
        nGene[geneCount1][geneCount2] =
          int((nMax-nMin)*u_distribution(u_generator))+nMin;}
    }
  }

  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  {
    for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
    {
      if(geneInteraction(geneCount1,geneCount2)==0)
      {
        lambdaGene[geneCount1][geneCount2]=0;
      } else {
        lambdaGene[geneCount1][geneCount2]=
        (lambdaMax-lambdaMin)*u_distribution(u_generator)+lambdaMin;}
    }
  }

  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  {
    for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
    {
      if(geneInteraction(geneCount1,geneCount2)==0)
      {
        threshGeneLog[geneCount1][geneCount2]=0;
      }
      else
      {

        threshGeneLog[geneCount1][geneCount2] =
          (1-sdFactor*std::sqrt(3))*thresholdGene[geneCount2] +
          (2*std::sqrt(3)*sdFactor*thresholdGene[geneCount2])
        *u_distribution(u_generator);
      }
    }
  }


}


void selectIcRange(const int numberGene, IntegerMatrix geneInteraction,
                    const std::vector<double> &gGene,
                    const std::vector<double> &kGene,
                    const std::vector<std::vector<double> > &lambdaGene,
                    std::vector<double> &maxGene,
                    std::vector<double> &minGene)
{
  for(int geneCount1=0;geneCount1<numberGene;geneCount1++){
    maxGene[geneCount1]=gGene[geneCount1]/kGene[geneCount1];}

  for(int geneCount1=0;geneCount1<numberGene;geneCount1++)
  {
    double minGene_multiplier_final=1;

    for(int geneCount2=0;geneCount2<numberGene;geneCount2++)
    {
      double gene_min_multiplier=1;
      double geneLambda=lambdaGene[geneCount1][geneCount2];
      switch(geneInteraction(geneCount1,geneCount2))
      {
      case 0:
        gene_min_multiplier=1.0;
        break;

      case 2:
        geneLambda=1./geneLambda;
        gene_min_multiplier=geneLambda;
        break;

      case 1:
        gene_min_multiplier=1./geneLambda;
        break;
      default :
        Rcout << "Invalid Interation code for Gene"<<geneCount1
        <<" and gene"<<geneCount2<<" interaction"<<"\n";
      }

      minGene_multiplier_final = minGene_multiplier_final*gene_min_multiplier;
    }

    minGene[geneCount1] =
      gGene[geneCount1]/kGene[geneCount1]*minGene_multiplier_final;

  }

}


// [[Rcpp::export]]

int simulateGRCCpp(Rcpp::IntegerMatrix geneInteraction,
                Rcpp::List config, String outFileGE, String outFileParams,
                String outFileIC,
              const int stepper = 1)

{
    unsigned int seed =  static_cast<unsigned int>
    (Rcpp::sample(32000,1,true)(0));
    std::mt19937_64 g_generator (seed);
    
  // Initialize the network
  size_t numberGene = geneInteraction.ncol();
  //vector containing tgtGene of nth interaction

  NumericVector simulationParameters =
    as<NumericVector>(config["simParams"]);
  NumericVector stochasticParameters =
    as<NumericVector>(config["stochParams"]);
  NumericVector hyperParameters =
    as<NumericVector>(config["hyperParams"]);
  LogicalVector options = as<LogicalVector>(config["options"]);
  NumericVector clampGenes =
    as<NumericVector>(config["clampGenes"]);
  NumericMatrix clampValues =
    as<NumericMatrix>(config["clampValues"]);
  
  // Rcout << "clampValues from simulateGrc.cpp:";
  // Iterate through each element in the matrix and print it
  //for (int i = 0; i < clampValues.nrow(); ++i) {
  //  for (int j = 0; j < clampValues.ncol(); ++j) {
  //    Rcpp::Rcout << "Element at [" << i << ", " << j << "]: " << clampValues(i, j) << std::endl;
  //  }
  //}
  //Rcout << "\n\n";
  
  // logic for clamping. if no clamps enabled, ignore this as clampMap is only a necessary param for the clamp stepper
  bool noClamps = std::all_of(clampGenes.begin(), clampGenes.end(), [](int i) { return i==0; });
  Rcout << "Noclamps: " << noClamps << "\n";
  //std::unordered_map<int, double> clampMap;
  std::unordered_map<int, std::vector<double>> clampMap;
  if(!noClamps) {
    int clampIdx = 0;
    for(int i = 0; i < numberGene; i++) {
      if(clampGenes[i] == 1) {
        //clampMap[i] = clampValues[clampIdx];
        // Create a vector to store the row elements
        std::vector<double> colVec(clampValues.nrow());
        // Copy the elements from the matrix row to the vector
        for (int j = 0; j < clampValues.nrow(); ++j) {
          colVec[j] = clampValues(j, clampIdx);
        }
        
        clampMap[i] = colVec;
        clampIdx++;
      }
    }
    
  }
  // Rcout << "Printing clamp map below: \n";
  // for (const auto& pair : clampMap) {
  //   Rcpp::Rcout << "Key: " << pair.first << ", Value: " << std::endl;
  //   for (double val : pair.second) {
  //     Rcpp::Rcout << val << ", ";
  //   }
  //   Rcpp::Rcout << std::endl;
  // }
  // 

  size_t numModels = static_cast<size_t>(simulationParameters[0]);
  double simulationTime = simulationParameters[1];
  double h = simulationParameters[2];
  double printStart = simulationParameters[3];
  size_t nIC = static_cast<size_t> (simulationParameters[4]);
  size_t outputPrecision = static_cast<size_t> (simulationParameters[5]);
  double rkTolerance = simulationParameters[6];
  // double paramRange = simulationParameters[7];
  double printInterval = simulationParameters[8];
  int sigGene = simulationParameters[9];
  double maxFC = simulationParameters[10];
  size_t nNoise = 1 + static_cast<size_t>(stochasticParameters[0]);
  double noiseScalingFactor = stochasticParameters[1];
  double initialNoise = stochasticParameters[2];
  double shotNoise = static_cast<double>(stochasticParameters[4]);
  double ou_tcorr = static_cast<double>(stochasticParameters[5]);
  //Rcout<<"stochastic param 5 on start of simulateGRCRcpp: "<<stochasticParameters[5]<<"\n";
  //Rcout<<"ou_tcorr on start of simulateGRCRcpp: "<<ou_tcorr<<"\n";

  double gMin = hyperParameters[0];
  double gMax = hyperParameters[1];
  double kMin = hyperParameters[2];
  double kMax = hyperParameters[3];
  double lambdaMin = hyperParameters[4];
  double lambdaMax = hyperParameters[5];
  size_t nMin = static_cast<size_t>(hyperParameters[6]);
  size_t nMax = static_cast<size_t>(hyperParameters[7]);
  size_t interactionTypes = static_cast<size_t>(hyperParameters[8]);
  // size_t thresholdModels = static_cast<size_t>(hyperParameters[9]);
  double sdFactor = hyperParameters[10];

  NumericVector thresholdGene = as<NumericVector>(config["thresholds"]);

  bool anneal = options[0];
  bool scaledNoise = options[1];
  bool genIC = options[2];
  bool genParams = options[3];
  bool integrate = options[4];
  bool simDet = options[5];
  // bool useBoost = options[5];
  // bool useBoost = true;

  // size_t maxSteps = static_cast<size_t>(simulationTime/h);

  std::string fileNameGE = outFileGE;
  std::string fileNameParam = outFileParams;
  std::string fileNameIC = outFileIC;



    double D=initialNoise; // setting noise to maximum noise level
    std::vector<double> Darray(numberGene);
    //array to scale the noise level in each gene
    //Rcout<<"parameter_file"<<parameters_file<<"\n";
    // if(parameters_file) Rcout<<"If true"<<"\n";
    // Scale the noise level in each gene if scaled_noise = 1
    if(scaledNoise){

      for(size_t i=0; i<numberGene;i++)
      {
        Darray[i]=thresholdGene[i];}
 //     Rcout<<"Using a noise level that is
 // proportional to median expression of the gene"<<"\n";

    }
    else{
      for(size_t i=0; i<numberGene;i++)
      {
        Darray[i]=1.0;
      }
//      Rcout<<"Using same noise level for each gene"<<"\n";

    }

    //Create output files if not there already
    std::ofstream outGE(fileNameGE, std::ios::out);
    if(!outGE.is_open()) {     Rcout << "Cannot open output file.\n";
      return 1;}

    std::ifstream inParams;
    std::ofstream outParam;

    if(genParams){
      outParam.open(fileNameParam,std::ios::out);
    }
    else {
      inParams.open(fileNameParam,
                         std::ifstream::in);
      if(!inParams.is_open()) {     Rcout <<fileNameParam
        << "Cannot open input file for reading parameters.\n";  return 1;
      }
    }

    std::ifstream inIC;
    std::fstream outIC;
    if(genIC){
      outIC.open(fileNameIC,std::ios::out);
    }
    else
    {
      inIC.open(fileNameIC,std::ifstream::in);
      if(!inIC.is_open()) {
        Rcout <<"Cannot open input file for reading initial conditions.\n";
        return 1;
        }
    }
    //  containerType state;
    std::vector<size_t> tgtGeneTmp;
    //vector containing source and type of nth interaction
    std::vector<std::pair<size_t,size_t> > intSrcTypeTmp;

  //  size_t  nInteractions = convertAdjMatToVector(geneInteraction,
  //                                                tgtGeneTmp, intSrcTypeTmp);

    for(size_t modelCount=0;modelCount<numModels;modelCount++)
      {
            if((static_cast<size_t>(10*modelCount) % numModels) == 0){
              Rcout<<"====";
            }

            // Check for user interrupt and exit if there is any.
        if (modelCount % 100 == 0)
          Rcpp::checkUserInterrupt();

        //Initialize production rate of genes
        std::vector<double> gGene(numberGene);

        //Initialize degradation rate of genes
        std::vector<double> kGene(numberGene);

        //Initialize hill coefficient for each interaction
        std::vector<std::vector<int> >
          nGene(numberGene, std::vector<int>(numberGene));

        //Initialize fold change for each interaction
        std::vector<std::vector<double> >
          lambdaGene(numberGene, std::vector<double>(numberGene));

        //Initialize threshold for each interaction
        std::vector<std::vector<double> >
          threshGeneLog(numberGene, std::vector<double>(numberGene));

        if(inParams.is_open())
        {
          readParameters( geneInteraction, numberGene, gGene,
                           kGene, nGene,
                           lambdaGene,
                           threshGeneLog, inParams);
        }
        else
        {

          selectParameters( geneInteraction,  thresholdGene,
                            gMin,  gMax,
                            kMin,  kMax,  interactionTypes,
                            lambdaMin,
                            lambdaMax,  nMin,  nMax,
                            sdFactor,  numberGene,
                            gGene,
                            kGene, nGene,
                            lambdaGene,
                            threshGeneLog);

          writeParameters( numberGene, outputPrecision, gGene,
                           kGene, nGene,
                           lambdaGene,
                           threshGeneLog, outParam);
        }


        /////////////////////////////////////////////////////////////////////

        //Initial condition range selection
        /////////////////////////////////////////////////////////////////////
        std::vector<double> maxGene(numberGene);
        std::vector<double> minGene(numberGene);
        if(genIC)
        {
          selectIcRange( numberGene,  geneInteraction, gGene, kGene,
                         lambdaGene, maxGene, minGene);
        }

        ///////////////////////////////////////////////////////////////////////

        //Initial condition  selection
        ///////////////////////////////////////////////////////////////////////
        for(size_t icCount=0;icCount<nIC;icCount++)
        {
          std::vector <double> expressionGene(numberGene);
          //array for current gene expression
          std::vector <double> expressionGene0(numberGene);
          //array for initial gene expression
          std::vector <double> prevNoise(numberGene);
          //array for current noise 
          if(!genIC)
          {
            for(size_t icCounter=0;icCounter <numberGene; icCounter++)
            {
              inIC >> expressionGene0[icCounter];
            }
          }

          else
          {
            for(size_t geneCount1=0;geneCount1<numberGene;geneCount1++)
            {
              expressionGene0[geneCount1]=exp(std::log(minGene[geneCount1]) +
                (std::log(maxGene[geneCount1]) -
                std::log(minGene[geneCount1]))*u_distribution(u_generator));

            }

            ///////////////////////////////////////////////////////////////////

            //Writing initial condition to file
            //////////////////////////////////////////////////////////////////

            for(size_t geneCount1=0;geneCount1<numberGene;geneCount1++)
            {
              outIC<<std::setprecision(outputPrecision)
              <<expressionGene0[geneCount1]<<"\t";
            }
            outIC<<"\n";

          }

          ///////////////////////////////////////////////////////////////////

          //Time Evolution
          ///////////////////////////////////////////////////////////////////
          D=initialNoise; //Start with maximum noise level for each model

          for(size_t geneCount1=0;geneCount1<numberGene;geneCount1++)
          {
            expressionGene[geneCount1]=expressionGene0[geneCount1];
            prevNoise[geneCount1]=0; // use 0 as initial noise value
          }

          for(size_t fileCount=0;fileCount<nNoise;fileCount++)
          {
            if((fileCount==nNoise-1) &(simDet)){D=0;}



            if(anneal) {}
            else {
              for(size_t geneCount_temp=0;geneCount_temp<numberGene;
              geneCount_temp++) {
                expressionGene[geneCount_temp] =
                  expressionGene0[geneCount_temp];
              }
            }
            if(integrate) {


            switch(stepper){
            case 1:
              // Euler Maruyama method
              stepEM( expressionGene, outGE, simulationTime,
                    numberGene, geneInteraction, gGene, kGene, nGene,
                    lambdaGene, threshGeneLog, interactionTypes,
                    sdFactor, shotNoise, Darray,
                    outputPrecision, printStart, printInterval, D, h);
              break;
            case 4:
              //fourth order Runge-Kutta
//              Rcout<<"RK4";
              stepRK4( expressionGene, outGE, simulationTime, numberGene,
                      geneInteraction, gGene, kGene, nGene, lambdaGene,
                       threshGeneLog, interactionTypes,
                       sdFactor,
                       outputPrecision,
                       printStart,  printInterval, h);
              break;

            case 5:
//              Rcout<<"DP";
              // adaptive Dormand Prince
              stepDP( expressionGene,outGE,simulationTime,numberGene,
                      geneInteraction,gGene,kGene,nGene,lambdaGene,
                      threshGeneLog,interactionTypes,
                      sdFactor,
                      outputPrecision,printStart, printInterval,h,
                      rkTolerance);
              break;
            case 6:
              //Rcout<<"EM_OU with tau="<<ou_tcorr;
              // EM with OU noise
              stepEM_OU( expressionGene, outGE, simulationTime,
                    numberGene, geneInteraction, gGene, kGene, nGene,
                    lambdaGene, threshGeneLog, interactionTypes,
                    sdFactor, shotNoise, Darray,
                    outputPrecision, printStart, printInterval, D, h,
                    ou_tcorr, prevNoise);
              // stepEM( expressionGene, outGE, simulationTime,
              //       numberGene, geneInteraction, gGene, kGene, nGene,
              //       lambdaGene, threshGeneLog, interactionTypes,
              //       sdFactor, shotNoise, Darray,
              //       outputPrecision, printStart, printInterval, D, h);
              break;

            case 7:
              stepEM_sig_grad( expressionGene, outGE, simulationTime,
                    numberGene, geneInteraction, gGene, kGene, nGene,
                    lambdaGene, threshGeneLog, interactionTypes,
                    sdFactor, shotNoise, Darray,
                    outputPrecision, printStart, printInterval, D, h, sigGene,
                    maxFC);
              break;

            case 8:
              stepRK4_sig_grad( expressionGene, outGE, simulationTime,
                    numberGene, geneInteraction, gGene, kGene, nGene,
                    lambdaGene, threshGeneLog, interactionTypes,
                    sdFactor, outputPrecision, printStart, printInterval, h,
                    sigGene, maxFC);
              break;

            case 9:
              //Rcout<<"EM_OU_Clamp with tau="<<ou_tcorr;
              stepEM_OU_Clamp( expressionGene, outGE, simulationTime,
                         numberGene, geneInteraction, gGene, kGene, nGene,
                         lambdaGene, threshGeneLog, interactionTypes,
                         sdFactor, shotNoise, Darray,
                         outputPrecision, printStart, printInterval, D, h,
                         ou_tcorr, prevNoise, clampMap, modelCount);
              break;
            default:
              Rcout<< "Error in specifying the stepper.\n";

            }
            }
            //Rcout<<"D="<<D<<"\n";
            // Rcout<< "Noise Level" << fileCount<<"\t"<<D<<"\n";
            D=D*noiseScalingFactor;

          }

          outGE<<"\n";
        }
      }


    outGE.close();
    outParam.close();
    outIC.close();
    if(inIC.is_open()) inIC.close();
    if(inParams.is_open()) inParams.close();

// Rcout<<"Simulations completed successfully.\n";

  return 0;
}

