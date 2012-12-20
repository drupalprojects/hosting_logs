<?php
/**
 * @file provision_logs.drush.inc
 * Separates sites error logs into individual files, and makes an Apache Alias
 * from /error.log to the error log path.
 *
 * Once downloaded into a drush command folder, simply re-verify any site.
 * Once verified, check http://yoursite/error.log to see the raw apache error
 * logs.
 */

/**
 * Implements drush_hook_pre_COMMAND()
 *
 * Creates the /var/aegir/logs folder if it does not exist.
 */
function drush_provision_logs_pre_provision_verify() {
  $log_path = drush_get_option('provision_logs_path', '/var/aegir/logs');
  if (!file_exists($log_path)){
    mkdir($log_path, 0775);
  }
}

/**
 * Implements hook_provision_apache_vhost_config()
 *
 * Sets each site's error log to be at /var/aegir/logs/$uri.error.log
 * Makes http://$uri/errors available through the browser.
 *
 * @TODO: Provide a way for sites to say they want the web-accessible alias or not
 */
function provision_logs_provision_apache_vhost_config($uri, $data) {
  $log_file_path = drush_get_option('provision_logs_file_path', '/var/aegir/logs');
  $log_url_path = drush_get_option('provision_logs_url_path', 'error.log');

  $error_log_path = "$log_file_path/$uri.error.log";

  $vhost = array();
  $vhost[] = "";
  $vhost[] = "# PROVISION_LOGS";
  $vhost[] = "# ==============";
  $vhost[] = "# Allow error log to be accessible from http://$uri/$log_url_path";
  $vhost[] = "Alias /$log_url_path $error_log_path";
  $vhost[] = "";
  $vhost[] = "# Set error log to site-specific file.";
  $vhost[] = "ErrorLog $error_log_path";
  $vhost[] = "# ==============";
  $vhost[] = "";
  return $vhost;
}
