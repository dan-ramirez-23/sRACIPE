rm(list=ls())


library(doFuture)
library(Rcpp)
# ### Install
.libPaths("/Users/danramirez/localR/4.2.2-arm")
.libPaths()
devtools::install(args="--verbose")

devtools::install_github("dan-ramirez-23/sRACIPE")


# Test on repressilator
topo <- data.frame(Source=c("A","B","C"), Target=c("B","C","A"), Type=c(2,2,2))


## Time series simulation
racipe <- sracipeSimulate(circuit=topo, numModels = 1, simulationTime = 20, 
                          integrateStepSize = 0.1, timeSeries=T, printStart=0.1, printInterval=0.1)



tsData <- racipe@metadata$timeSeries
