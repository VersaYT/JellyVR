{
  "godot_version": "4.4",
  "version_string": "{{ context.release.tag_name }}",
  "download_provider": "GitHub",
  "download_commit": "{{ env.GITHUB_SHA }}",
  "browse_url": "{{ context.repository.html_url }}",
  "issues_url": "{{ context.repository.html_url }}/issues"
}
