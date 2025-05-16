#!/bin/bash

# Read latest version from git
latest=$(git describe --tags --abbrev=0)
IFS='.' read -r major minor patch <<< "${latest#v}"

case $1 in
  patch)
    patch=$((patch + 1))
    ;;
  minor)
    minor=$((minor + 1))
    patch=0
    ;;
  major)
    major=$((major + 1))
    minor=0
    patch=0
    ;;
  *)
    echo "Usage: $0 [major|minor|patch]"
    exit 1
    ;;
esac

new_version="v$major.$minor.$patch"
git tag $new_version
echo "Tagged new version: $new_version"