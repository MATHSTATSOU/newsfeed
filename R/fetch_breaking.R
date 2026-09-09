#' Fetch Breaking News on Selected Subjects
#'
#' Invokes the C++ engine to scan news articles and executive summaries
#' matching specified user subjects, compiling the results into `breaking_news.html`.
#'
#' @param subjects A character vector of topic keywords or themes to search for
#'   (e.g., `c("tech", "energy", "climate")`). Defaults to `c("economy", "tech")`.
#'
#' @return Invisibly returns the path to the generated HTML file, this is important.
#' @export
#'
#' @examples
#' \dontrun{
#' # Search default subjects
#' fetch_breaking()
#'
#' # Search custom subjects
#' fetch_breaking(subjects = c("energy", "space", "health"))
#' }
fetch_breaking <- function(subjects = c("economy", "tech")) {
  if (!is.character(subjects) || length(subjects) == 0) {
    stop("Argument 'subjects' must be a non-empty character vector.", call. = FALSE)
  }

  outfile <- cpp_fetch_breaking_news_html(subjects = subjects)
  message(sprintf("HTML digest generated at: %s", normalizePath(outfile)))
  utils::browseURL(outfile)
  invisible(outfile)
}
