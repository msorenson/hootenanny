/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2013, 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.osm;

import java.sql.Connection;

import hoot.services.db.DbUtils;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;

/**
 * Service endpoint for authenticated OSM user detail information
 */
@Path("api/0.6/user/details")
public class UserDetailsResource
{
  private static final Logger log = LoggerFactory.getLogger(UserDetailsResource.class);

  @SuppressWarnings("unused")
  private ClassPathXmlApplicationContext appContext;

  public UserDetailsResource()
  {
    log.debug("Reading application settings...");
    appContext = new ClassPathXmlApplicationContext(new String[] { "db/spring-database.xml" });
  }

	/**
	 * <NAME>User Details Service </NAME>
	 * <DESCRIPTION>
	 * 	The Hootenanny user services implements the methods of the OSM User service v0.6 and OSM User Details service v0.6.
	 * Hootenanny has no authentication mechanism implemented yet. This service exists for now only to avoid modifications to
	 * the iD editor. In the future the Hootenanny OSM services will likely support OAuth 2.0 and this service will have purpose.
	 * The service methods first attempt to parse the request user identification data as a numerical user ID, and then, if unsuccessful,
	 *  attempts to parse it as a user name string.
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	XML representation of the user
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/osm/user/test</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>
	 * 	XML representation of the user
	 * see https://insightcloud.digitalglobe.com/redmine/projects/hootenany/wiki/User_-_OsmUserService#User-Details
	 * </OUTPUT>
	 * </EXAMPLE>
	 *
   * Service method endpoint for retrieving OSM user detail information for the authenticated user
   * associated with the request
   *
   * @return Response with user detail information
   * @throws Exception
   * @todo update to get actual logged in user once OAuth is implemented
   * @see https://insightcloud.digitalglobe.com/redmine/projects/hootenany/wiki/User_-_OsmUserService#User-Details
   */
  @GET
  @Consumes(MediaType.TEXT_PLAIN)
  @Produces(MediaType.TEXT_XML)
  public Response getDetails() throws Exception
  {
    log.debug("Retrieving logged in user details...");

    //TODO: finish - For now, we're just grabbing the first user in the db, since we don't have
    //any authentication in place to get the correct user.  Worst case, for now, you see incorrect
    //user information from iD editor...not a big deal since authentication doesn't exist anyway.
    Connection conn = DbUtils.createConnection();
    long userId = -1;
    try
    {
      log.debug("Initializing database connection...");
      userId = DbUtils.getTestUserId(conn);
    }
    finally
    {
    	DbUtils.closeConnection(conn);
    }
    assert(userId != -1);
    return (new UserResource()).get(String.valueOf(userId));
  }
}
