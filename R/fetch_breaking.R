#' Fetch World Breaking News to HTML
#'
#' Executes the underlying C++ fetch engine to gather current breaking
#' global headlines and write them to a formatted `breaking_news.html` file.
#'
#' @return Invisibly returns the file path of the generated HTML report.
#' @export
#'
#' @examples
#' \dontrun{
#' fetch_breaking()
#' }
fetch_breaking <- function() {
  outfile <- cpp_fetch_breaking_news_html()
  message(sprintf("HTML news feed successfully generated at: %s", normalizePath(outfile)))
  utils::browseURL(outfile)
  invisible(outfile)
}
