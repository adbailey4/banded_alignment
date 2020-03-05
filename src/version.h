/// Current version of the banded_alignment application.
///
/// This project uses the Semantic Versioning scheme:
///
/// <http://semver.org/>
///
/// Major versions introduce significant changes to the API, and backwards
/// compatibility is not guaranteed. Minor versions are for new features and
/// other backwards-compatible changes to the API. Patch versions are for bug
/// fixes and internal code changes that do not affect the API. A "dev" version
/// is a pre-release version, e.g. "1.0.0dev" is part of the development cycle
/// for the "1.0.0" release.
///
/// Release versions *MUST* uniquely identify a single VCS commit, and *MUST*
/// be tagged, e.g. "v1.0.0". The only change allowed for a release commit
/// is incrementing the version to the release number, which minimizes the
/// chance of introducing a breaking change to the final pre-release version.
/// The next commit following a release commit *MUST* include an incremented
/// version number, e.g. "1.0.0" => "1.0.1dev".
///
/// Version 0.x should be considered a development version with an unstable
/// API, and backwards compatibility is not guaranteed for minor versions.
///
#ifndef BANDED_ALIGNMENT_VERSION_HPP
#define BANDED_ALIGNMENT_VERSION_HPP

#define BANDED_ALIGNMENT_VERSION "0.1.0.0"

#endif  // BANDED_ALIGNMENT_VERSION_HPP
