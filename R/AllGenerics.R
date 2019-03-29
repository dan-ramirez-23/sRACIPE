
#' @export
#' @import SummarizedExperiment
#' @import SummarizedExperiment
#' @title  Method to get the circuit
#' @description The circuit file should contain three columns with headers,
#' "Source" "Target" "Type"
#' Here "Source" and "Target" are the names of the genes and "Type" refers to
#' the regulation, "1" if source activates target and "2" if source inhibits
#' target.
#' @param .object RacipeSE object
#'
#' @return A dataframe
#' @examples
#' rs <- RacipeSE()
#' data("demoCircuit")
#' circuit(rs) <- demoCircuit
#' circuitDataFrame <- circuit(rs)
#'

setGeneric(name="circuit",
           def=function(.object)
           {
             standardGeneric("circuit")
           }
)


#' @export
#' @title Initialize the circuit
#' @description Initialize the circuit from a topology file
#' or a \code{data.frame}
#' A typical topology file looks like
#' \tabular{lcr}{
#'   Source \tab Target \tab Type \cr
#'   geneA \tab geneB \tab 2 \cr
#'   geneB \tab geneC \tab 1 \cr
#'   geneB \tab geneA \tab 2
#' }
#' Here the regulation type is specified by number - activation: \code{1},
#'  inhibition: \code{2}
#' @param .object RacipeSE object
#' @param value data.frame containing the circuit information
#' @return data.frame
#' @examples
#' RacipeSet <- RacipeSE()
#' data("demoCircuit")
#' circuit(RacipeSet) <- demoCircuit
#' circuit(RacipeSet)
#' @section Related Functions:
#'
#' \code{\link{simulateGRC}},  \code{\link{knockDownAnalysis}},
#' \code{\link{overExprAnalysis}},  \code{\link{plotData}}
#'

setGeneric("circuit<-",
           def = function(.object, value)
           {
             standardGeneric("circuit<-")
           }
)

#' @export
#' @import SummarizedExperiment
#' @title  A method to access the simulation hyperparameters
#' @description The hyperparameters like number of models, range from which
#' parameters are to be sampled, simulation time etc.
#' @param .object RacipeSE object
#' @examples
#' RacipeSet <- RacipeSE()
#' data("demoCircuit")
#' circuit(RacipeSet) <- demoCircuit
#' config(RacipeSet)
#' @return list
#'

setGeneric(name="config",
           def=function(.object)
           {
             standardGeneric("config")
           }
)

#' @export
#' @import SummarizedExperiment
#' @title  A method to access the simulation hyperparameters
#' @description The hyperparameters like number of models, range from which
#' parameters are to be sampled, simulation time etc.
#' @param .object RacipeSE object
#' @param value list. Configuration as a list
#' @examples
#' rSet <- RacipeSE()
#' tmpConfig <- config(rSet)
#' config(rSet) <- tmpConfig
#' @return \code{RacipeSE} object
#'

setGeneric("config<-",
           def = function(.object, value)
           {
             standardGeneric("config<-")
           }
)


#' @export
#' @import SummarizedExperiment
#' @importFrom S4Vectors metadata
#' @title A method to access the simulation parameters
#' @description The parameters for each model.
#' @param .object RacipeSE object
#' @param ... additional arguments
#' @examples
#' data("demoCircuit")
#' RacipeSet <- simulateGRC(demoCircuit)
#' parameters <- params(RacipeSet)
#' params(RacipeSet) <- parameters
#' @return A  data.frame
#'
setGeneric("params",
           def = function(.object)
           {
             standardGeneric("params")
           }
)


#' @export
#' @title  A method to set the simulation parameters
#' @description The circuit file should contain three columns with headers,
#' "Source" "Target" "Type"
#' Here "Source" and "Target" are the names of the genes and "Type" refers to
#' the regulation, "1" if source activates target and "2" if source inhibits
#' target.
#' @param .object RacipeSE object
#' @param value DataFrame containing the parameteres
#' @examples
#' data("demoCircuit")
#' rSet <- sRACIPE::simulateGRC(circuit = demoCircuit, numModels = 100)
#' parameters <- params(rSet)
#' params(rSet) <- parameters
#' @return A RacipeSE object

setGeneric("params<-",
           def = function(.object, value)
           {
             standardGeneric("params<-")
           }
)



#' @export
#' @import SummarizedExperiment
#' @title  A method to get the initial conditions used for simulations
#' @description The initial conditions of each of the models.
#' @param .object RacipeSE object
#' @examples
#' data("demoCircuit")
#' rSet <- sRACIPE::simulateGRC(circuit = demoCircuit, numModels = 100)
#' ics <- ic(rSet)
#' ic(rSet) <- ics
#' @return DataFrame

setGeneric("ic",
           def = function(.object)
           {
             standardGeneric("ic")
           }
)


#' @export
#' @import SummarizedExperiment
#' @title  A method to set the initial conditions
#' @description The circuit file should contain three columns with headers,
#' "Source" "Target" "Type"
#' Here "Source" and "Target" are the names of the genes and "Type" refers to
#' the regulation, "1" if source activates target and "2" if source inhibits
#' target.
#' @param .object RacipeSE object
#' @param value DataFrame containing the initial conditions
#' @examples
#' data("demoCircuit")
#' rSet <- sRACIPE::simulateGRC(circuit = demoCircuit, numModels = 100)
#' ics <- ic(rSet)
#' ic(rSet) <- ics
#' @return A RacipeSE object

setGeneric("ic<-",
           def = function(.object, value)
           {
             standardGeneric("ic<-")
           }
)



#' @export
#' @import SummarizedExperiment
#' @importFrom S4Vectors SimpleList
#' @title  Normalize the simulated gene expression
#' @description Normalize the simulated gene expression including gene
#' expressions for stochastic and knockout simulations
#' @param .object RacipeSE object
#'
#' @return A RacipeSE object
#' @examples
#' data("demoCircuit")
#' rSet <- sRACIPE::simulateGRC(circuit = demoCircuit, numModels = 100)
#' rSet <- normalizeGE(rSet)
#' @section Related Functions:
#'
#' \code{\link{simulateGRC}},  \code{\link{knockDownAnalysis}},
#' \code{\link{overExprAnalysis}},  \code{\link{plotData}},
#'
setGeneric("normalizeGE",
           def = function(.object)
           {
             standardGeneric("normalizeGE")
           }
)


#' @export
#' @import grDevices
#' @title Plot Gene Regulatory Circuit
#' @description  Plot Gene Regulatory Circuit to a file or output device.
#' @param .object RacipeSE object
#' A list returned by \code{\link{simulateGRC}} function
#' @param plotToFile (optional) logical. Default \code{FALSE}. Whether to save
#' plots to a file.
#' @examples
#' data("demoCircuit")
#' rSet <- sRACIPE::simulateGRC(circuit = demoCircuit, numModels = 100)
#' plotCircuit(rSet, plotToFile = FALSE)
#' @return circuit plot
#' @section Related Functions:
#'
#' \code{\link{simulateGRC}},  \code{\link{knockDownAnalysis}},
#' \code{\link{overExprAnalysis}},  \code{\link{plotData}}
setGeneric("plotCircuit",
           def = function(.object, plotToFile = FALSE)
           {
             standardGeneric("plotCircuit")
           }
)


#' @export
#' @import SummarizedExperiment
#' @importFrom gplots heatmap.2
#' @import ggplot2
#' @import gridExtra
#' @import umap
#' @import grDevices
#' @title Plot sRACIPE data
#' @description Plots heatmap, pca, umap of the data simulated using sRACIPE
#' @param  .object List A list returned by \code{\link{simulateGRC}} function
#' @param plotToFile (optional) logical. Default \code{FALSE}. Whether to save
#' plots to a file.
#' @param nClusters (optional) Integer. Default 2. Expected number of clusters
#' in the simulated data. Hierarchical clustering will be used to cluster the
#' data and the the models will be colored in UMAP and PCA plots according to
#' these clustering results. The clusters can be also supplied using
#' \code{assignedClusters}.
#' @param pcaPlot (optional) logical. Default \code{TRUE}. Whether to plot PCA
#' embedding.
#' @param umapPlot (optional) logical. Default \code{TRUE}. Whether to plot
#' UMAP embedding
#' @param networkPlot (optional) logical. Default \code{TRUE}. Whether to plot
#' the network.
#' @param clustMethod (optional) character. Default \code{"ward.D2"}. Clustering
#' method for heatmap. See \code{\link[gplots]{heatmap.2}}
#' @param col (optional) Color palette
#' @param distType (optional) Distance type.  Used only if specified
#' explicitly. Otherwise, 1-cor is used. See \code{\link[stats]{dist}},
#' \code{\link[stats]{hclust}}
#' @param assignedClusters vector integer or character. Default NULL.
#' Cluster assignment of models.
#' @param corMethod (optional) character. Default \code{"spearman"}. Correlation
#' method for distance function.
#' @param ... Other arguments
#' @examples
#' data("demoCircuit")
#' rSet <- sRACIPE::simulateGRC(circuit = demoCircuit, numModels = 100)
#' rSet <- plotData(rSet)
#' @return \code{RacipeSE} object
#' @section Related Functions:
#'
#' \code{\link{simulateGRC}},  \code{\link{knockDownAnalysis}},
#' \code{\link{overExprAnalysis}},  \code{\link{plotData}},
setGeneric("plotData",
           def = function(.object, plotToFile = FALSE, nClusters = 2,
                          pcaPlot = TRUE, umapPlot = TRUE,networkPlot = TRUE,
                          clustMethod = "ward.D2", col = col,
                          distType = "euclidean",
                          assignedClusters = NULL, corMethod = "spearman", ...)
           {
             standardGeneric("plotData")
           }
)

#' @export
#' @import SummarizedExperiment
#' @import ggplot2
#' @import grDevices
#' @title Parameter bifurcation plots
#' @description Plot the expression of the genes against parameter values
#' to understand the effect of parameters on the gene expressions.
#' @param .object RacipeSE object generated by
#' \code{\link{simulateGRC}} function.
#' @param paramName character. The name of the parameter to be plotted.
#' @param plotToFile (optional) logical. Default \code{FALSE}. Whether to save
#' plots to a file.
#' @param data (optional) dataframe. Default rSet geneExpression. The data
#' to be plotted. For example, use rSet$stochasticSimulations$[noise] to plot
#' the stochastic data.
#' @param paramValue (optional) Dataframe. The parameter values if rSet$params
#' is not to be used.
#' @param assignedClusters (optional) Dataframe. The cluster assignment of data.
#' @return none
#' @examples
#' data("demoCircuit")
#' rSet <- sRACIPE::simulateGRC(circuit = demoCircuit, numModels = 100,
#' plots=FALSE, plotToFile = FALSE)
#' rSet <- sRACIPE::normalizeGE(rSet)
#' plotParamBifur(rSet, "G_A")
#'
setGeneric("plotParamBifur",
           def = function(.object, paramName, data = NULL,
                          paramValue = NULL, assignedClusters = NULL,
                          plotToFile = FALSE)
           {
             standardGeneric("plotParamBifur")
           }
)


#' @export
#' @import SummarizedExperiment
#' @import ggplot2
#' @import grDevices
#' @title Perform in-silico over expression analysis
#' @description Calculates the fraction of models in different clusters
#' with full parameter
#' range and on a subset of models with high production rate of a specific gene
#'  representing the over expression of the specific gene.
#' @param .object RacipeSE object generated by
#' \code{\link{simulateGRC}} function.
#' @param clusterOfInterest (optional) cluster number (integer)
#' to be used for arranging
#' the transcription factors
#' @param overProduction (optional) Percentage to which production rate
#' decreases on knockdown. Uses a default value of 10 percent.
#' @param nClusters (optional) Number of clusters in the data. Uses a default
#' value of 2.
#' @param plotFilename (optional) Name of the output file.
#' @param plotHeatmap logical. Default TRUE. Whether to plot the heatmap or not.
#' @param plotBarPlot logical. Default TRUE. Whether to plot the barplot.
#' @param clusterCut integer or character. The cluster assignments.
#' @param plotToFile logical. Default FALSE.
#' @return List containing fraction of models in different clusters
#'  in the original simulations and after knowcking down different genes.
#' Additionaly, it generates two pdf files in the results folder.
#' First is barplot
#' showing the percentage of different clusters in the original simulations
#' and after knocking down each gene. The second pdf contains the heatmap of
#' clusters after marking the models with cluster assignments.
#' @examples
#' data("demoCircuit")
#' rSet <- sRACIPE::simulateGRC(circuit = demoCircuit, numModels = 100,
#' plots=FALSE, plotToFile = FALSE)
#' rSet <- sRACIPE::normalizeGE(rSet)
#' @section Related Functions:
#'
#' \code{\link{simulateGRC}},  \code{\link{knockDownAnalysis}},
#' \code{\link{overExprAnalysis}},  \code{\link{plotData}},
setGeneric("overExprAnalysis",
           def = function(.object, overProduction = 10,
                          nClusters = 2,
                          clusterOfInterest = 2,
                          plotFilename = NULL,
                          plotHeatmap = TRUE,
                          plotBarPlot = TRUE,
                          clusterCut = NULL,
                          plotToFile = FALSE)
           {
             standardGeneric("overExprAnalysis")
           }
)


#' @export
#' @import SummarizedExperiment
#' @importFrom stats cor as.dist hclust cutree as.dendrogram prcomp sd
#' @import ggplot2
#' @import grDevices
#' @title Perform in-silico knockdown analysis
#' @description  Calculate the fraction of models in different clusters
#' with full parameter
#' range and on a subset of models with low production rate of a specific gene
#'  representing the knockdown of the specific gene.
#' @param .object RacipeSE object generated by \code{\link{simulateGRC}}
#'  function.
#' @param clusterOfInterest (optional) cluster number (integer)
#' to be used for arranging
#' the transcription factors
#' @param reduceProduction (optional) Percentage to which production rate
#' decreases on knockdown. Uses a default value of 10 percent.
#' @param nClusters (optional) Number of clusters in the data. Uses a default
#' value of 2.
#' @param plotFilename (optional) Name of the output file.
#' @param plotHeatmap logical. Default TRUE. Whether to plot the heatmap or not.
#' @param plotBarPlot logical. Default TRUE. Whether to plot the barplot.
#' @param clusterCut integer or character. The cluster assignments.
#' @param plotToFile logical. Default FALSE.
#' @return List containing fraction of models in different clusters
#'  in the original simulations and after knowcking down different genes.
#' Additionaly, it generates two pdf files in the results folder.
#' First is barplot
#' showing the percentage of different clusters in the original simulations
#' and after knocking down each gene. The second pdf contains the heatmap of
#' clusters after marking the models with cluster assignments.
#' @examples
#' data("demoCircuit")
#' rSet <- sRACIPE::simulateGRC(circuit = demoCircuit, numModels = 100,
#' plots=FALSE, plotToFile = FALSE)
#' rSet <- sRACIPE::normalizeGE(rSet)
#' rSet <- sRACIPE::knockDownAnalysis(rSet, plotToFile = FALSE,
#' plotBarPlot = TRUE, plotHeatmap = FALSE, reduceProduction = 50)
#'@section Related Functions:
#'
#' \code{\link{simulateGRC}},  \code{\link{knockDownAnalysis}},
#' \code{\link{overExprAnalysis}},  \code{\link{plotData}}
#'
setGeneric("knockDownAnalysis",
           def = function(.object, reduceProduction = 10,
                          nClusters = 2,
                          clusterOfInterest = 2,
                          plotFilename = NULL,
                          plotHeatmap = TRUE,
                          plotBarPlot = TRUE,
                          clusterCut = NULL,
                          plotToFile = FALSE)
           {
             standardGeneric("knockDownAnalysis")
           }
)

