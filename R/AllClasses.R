#' @export
#' @import methods
#' @import SummarizedExperiment
#' @title RacipeSE
#' @description An S4 class for Random Circuit Perturbation
#' simulations of networks. Extends the SummarizedExperiment class
#'
#' @seealso \code{\link[SummarizedExperiment]{SummarizedExperiment}} for
#' SummarizedExperiment documentation
#'
.RacipeSE <- setClass("RacipeSE",
                      contains = "SummarizedExperiment")

#' @export
#' @title RacipeSE object construct
#' @importFrom utils data
#' @description Create an RacipeSE object
#' @import SummarizedExperiment
#' @importFrom S4Vectors metadata
#' @param ... Other arguments
#' @return RacipeSE object
#' @examples
#' rSet <- RacipeSE()
#'

RacipeSE <- function(...) {
    .object <- SummarizedExperiment(...)
    if (is.null(metadata(.object)$config)) {
        configData <- NULL
        utils::data("configData",envir = environment())
        metadata(.object)$config <- configData
    }
    .RacipeSE(.object)
}
